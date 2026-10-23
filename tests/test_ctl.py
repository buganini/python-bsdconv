import gc
import io
import os
from pathlib import Path
import socket
import tempfile
import unittest
from unittest.mock import patch

from bsdconv import Bsdconv


class CtlTests(unittest.TestCase):
    def setUp(self):
        self.directory = tempfile.TemporaryDirectory()
        self.addCleanup(self.directory.cleanup)
        self.root = Path(self.directory.name)
        self.environment = patch.dict(
            os.environ, {"BSDCONV_SCORE": str(self.root / "default-score")}
        )
        self.environment.start()
        self.addCleanup(self.environment.stop)

    def converter(self):
        converter = Bsdconv("utf-8:score-train:null")
        self.assertTrue(converter, Bsdconv.error())
        return converter

    def test_original_files_can_close_before_conversion(self):
        converter = self.converter()
        score = self.root / "score"
        output = self.root / "output"
        with score.open("w+b") as source, output.open("wb") as target:
            # Attachment must flush Python's pending writes first.
            target.write(b"prefix")
            converter.ctl(Bsdconv.CTL_ATTACH_SCORE, source, 0)
            converter.ctl(Bsdconv.CTL_ATTACH_OUTPUT_FILE, target, 0)
        converter.conv("A")
        self.assertEqual(score.read_bytes(), bytes(65) + b"\x01")
        self.assertEqual(output.read_bytes(), b"prefix\x00\x00\x00A")
        # Updating a score must seek, rather than append another byte.
        converter.conv("A")
        self.assertEqual(score.read_bytes(), bytes(65) + b"\x02")

    def test_native_wrapper_can_be_released(self):
        converter = self.converter()
        score = self.root / "score"
        source = Bsdconv.fopen(str(score), "w+")
        converter.ctl(Bsdconv.CTL_ATTACH_SCORE, source, 0)
        del source
        gc.collect()
        converter.conv("A")
        self.assertEqual(score.read_bytes(), bytes(65) + b"\x01")

    def test_replace_and_detach_output(self):
        converter = self.converter()
        first, second = self.root / "first", self.root / "second"
        with first.open("wb") as output:
            converter.ctl(Bsdconv.CTL_ATTACH_OUTPUT_FILE, output, 0)
        converter.conv("A")
        with second.open("wb") as output:
            converter.ctl(Bsdconv.CTL_ATTACH_OUTPUT_FILE, output, 0)
        converter.conv("B")
        converter.ctl(Bsdconv.CTL_ATTACH_OUTPUT_FILE, None, 0)
        converter.conv("C")
        self.assertEqual(first.read_bytes(), b"\x00\x00\x00A")
        self.assertEqual(second.read_bytes(), b"\x00\x00\x00B")

    def test_bad_file_does_not_replace_existing_attachment(self):
        converter = self.converter()
        output = self.root / "output"
        with output.open("wb") as target:
            converter.ctl(Bsdconv.CTL_ATTACH_OUTPUT_FILE, target, 0)
        with self.assertRaises(io.UnsupportedOperation):
            converter.ctl(Bsdconv.CTL_ATTACH_OUTPUT_FILE, io.StringIO(), 0)
        with self.assertRaises(ValueError):
            converter.ctl(Bsdconv.CTL_ATTACH_OUTPUT_FILE, target, 0)
        with output.open("rb") as readonly:
            with self.assertRaises((ValueError, OSError)):
                converter.ctl(Bsdconv.CTL_ATTACH_OUTPUT_FILE, readonly, 0)
        converter.conv("A")
        self.assertEqual(output.read_bytes(), b"\x00\x00\x00A")

    def test_reject_invalid_native_file_and_score(self):
        converter = self.converter()
        invalid = Bsdconv.fopen(str(self.root / "missing"), "r")
        with self.assertRaises(ValueError):
            converter.ctl(Bsdconv.CTL_ATTACH_OUTPUT_FILE, invalid, 0)
        with self.assertRaises(ValueError):
            converter.ctl(Bsdconv.CTL_ATTACH_SCORE, None, 0)
        with (self.root / "append").open("a+b") as source:
            with self.assertRaises(ValueError):
                converter.ctl(Bsdconv.CTL_ATTACH_SCORE, source, 0)

    def test_type_check_exceptions_propagate(self):
        class Broken:
            @property
            def __class__(self):
                raise RuntimeError("class lookup failed")

        converter = self.converter()
        with self.assertRaisesRegex(RuntimeError, "class lookup failed"):
            converter.ctl(Bsdconv.CTL_ATTACH_OUTPUT_FILE, Broken(), 0)
        with self.assertRaises(TypeError):
            converter.ctl(Bsdconv.CTL_ATTACH_OUTPUT_FILE, object(), 0)

    @unittest.skipUnless(os.name == "posix", "requires POSIX socket descriptors")
    def test_score_must_be_seekable(self):
        converter = self.converter()
        first, second = socket.socketpair()
        with first, second, first.makefile("rwb", buffering=0) as source:
            with self.assertRaises(OSError):
                converter.ctl(Bsdconv.CTL_ATTACH_SCORE, source, 0)

    def test_spoofed_native_type_is_rejected(self):
        native = Bsdconv.fopen(str(self.root / "native"), "w+")

        class Spoof:
            @property
            def __class__(self):
                return type(native)

        converter = self.converter()
        with self.assertRaises(TypeError):
            converter.ctl(Bsdconv.CTL_ATTACH_OUTPUT_FILE, Spoof(), 0)

    def test_flush_error_propagates(self):
        class BrokenFlush(io.FileIO):
            def flush(self):
                raise RuntimeError("flush failed")

        converter = self.converter()
        source = BrokenFlush(self.root / "broken", "w")
        try:
            with self.assertRaisesRegex(RuntimeError, "flush failed"):
                converter.ctl(Bsdconv.CTL_ATTACH_OUTPUT_FILE, source, 0)
        finally:
            with self.assertRaises(RuntimeError):
                source.close()

    def test_invalid_converter_and_nonfile_control(self):
        with self.assertRaises(ValueError):
            Bsdconv("no-such-codec").ctl(Bsdconv.CTL_AMBIGUOUS_PAD, None, 1)
        converter = Bsdconv("utf-8:utf-8")
        self.assertTrue(converter.ctl(Bsdconv.CTL_AMBIGUOUS_PAD, None, 1))

    @unittest.skipUnless(Path("/dev/fd").is_dir(), "requires descriptor listing")
    def test_descriptors_released_on_replacement_and_destruction(self):
        gc.collect()
        before = len(os.listdir("/dev/fd"))
        converter = self.converter()
        # The library also holds descriptors for its loaded codec tables.
        initialized = len(os.listdir("/dev/fd"))
        for _ in range(40):
            with (self.root / "output").open("wb") as target:
                converter.ctl(Bsdconv.CTL_ATTACH_OUTPUT_FILE, target, 0)
        self.assertEqual(len(os.listdir("/dev/fd")), initialized + 1)
        target_stat = (self.root / "output").stat()
        for entry in os.listdir("/dev/fd"):
            fd = int(entry)
            try:
                status = os.fstat(fd)
            except OSError:  # The descriptor used by listdir has closed.
                continue
            if (status.st_dev, status.st_ino) == (target_stat.st_dev, target_stat.st_ino):
                self.assertFalse(os.get_inheritable(fd))
        converter.ctl(Bsdconv.CTL_ATTACH_OUTPUT_FILE, None, 0)
        self.assertEqual(len(os.listdir("/dev/fd")), initialized)
        del converter
        gc.collect()
        self.assertEqual(len(os.listdir("/dev/fd")), before)


if __name__ == "__main__":
    unittest.main()

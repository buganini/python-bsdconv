Python binding API
==================

.. py:module:: bsdconv

.. py:class:: Bsdconv

	.. py:data:: FILTER
	.. py:data:: FROM
	.. py:data:: INTER
	.. py:data:: TO

		Phase type

	.. py:data:: CTL_ATTACH_SCORE
	.. py:data:: CTL_ATTACH_OUTPUT_FILE
	.. py:data:: CTL_AMBIGUOUS_PAD

		Request for :py:meth:`ctl`

	.. py:method:: init()

		Initialize/Reset bsdconv converter

	.. py:method:: ctl(command, file, value)

		Manipulate the underlying codec parameters

		``file`` accepts an ``io.IOBase`` with a file descriptor, a
		``Bsdconv_file``, or ``None`` for commands without a file argument.
		``CTL_ATTACH_SCORE`` requires a seekable file opened for reading and
		writing, without append mode. ``CTL_ATTACH_OUTPUT_FILE`` requires a
		writable file; passing ``None`` detaches its current output file.

		The converter flushes the supplied file and owns a duplicate descriptor
		until it is replaced, detached, or the converter is destroyed. The
		caller may close the original file after attachment. The descriptors
		share their file position, so do not use the original file for I/O
		while it is attached. Invalid files raise an exception without replacing
		the existing attachment.

	.. py:method:: conv(s)

		Perform conversion

	.. py:method:: conv_chunk(s)

		Perform conversion without initializing and flushing

	.. py:method:: conv_chunk_last(s)

		Perform conversion without initializing

	.. py:method:: conv_file(from_file, to_file)

		Perform conversion with given filename

	.. py:method:: testconv(s)

		Perform test conversion

	.. py:method:: testconv_chunk(s)

		Perform test conversion without initializing and flushing

	.. py:method:: testconv_chunk_last(s)

		Perform test conversion without initializing

	.. py:method:: testconv_file(from_file)

		Perform test conversion with given filename

	.. py:method:: counter([name])

		Return counter or counters if not specified

	.. py:method:: counter_reset([name])

		Reset counter, if no name supplied, all counters will be reset

	.. py:staticmethod:: insert_phase(conversion, codecs, phase_type, phasen)

		Insert conversion phase into bsdconv conversion string

	.. py:staticmethod:: insert_codec(conversion, codec, phasen, codecn)

		Insert conversion codec into bsdconv conversion string

	.. py:staticmethod:: replace_phase(conversion, codecs, phase_type, phasen)

		Replace conversion phase in the bsdconv conversion string

	.. py:staticmethod:: replace_codec(conversion, codec, phasen, codecn)

		Replace conversion codec in the bsdconv conversion string

	.. py:staticmethod:: error()

		Return error message

	.. py:staticmethod:: mktemp()

		Make temporary file

	.. py:staticmethod:: fopen()

		Open file

	.. py:staticmethod:: codecs_list(type)

		DEPRECATED: Use :py:meth:`modules_list` instead

	.. py:staticmethod:: modules_list(type)

		Get modules list of specified type

	.. py:staticmethod:: codec_check(type, module)

		DEPRECATED: Use :py:meth:`module_check` instead

	.. py:staticmethod:: module_check(type, module)

		Check availability with given type and module name

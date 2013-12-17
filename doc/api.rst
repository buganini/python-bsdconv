Python binding API
==================

.. py:module:: bsdconv

.. py:function:: insert_phase(conversion, codecs, phase_type, phasen)

	Insert conversion phase into bsdconv conversion string

.. py:function:: insert_codec(conversion, codec, phasen, codecn)

	Insert conversion codec into bsdconv conversion string

.. py:function:: replace_phase(conversion, codecs, phase_type, phasen)

	Replace conversion phase in the bsdconv conversion string

.. py:function:: replace_codec(conversion, codec, phasen, codecn)

	Replace conversion codec in the bsdconv conversion string

.. py:function:: error()

	Return error message

.. py:function:: mktemp()

	Make temporary file

.. py:function:: fopen()

	Open file

.. py:function:: codecs_list()

	list codecs

.. py:function:: codec_check(type, codec)

	check if a codec is available


.. py:class:: Bsdconv

	.. py:method:: init()

		Initialize/Reset bsdconv instance

	.. py:method:: ctl(arg_ptr_obj, arg_int)

		Initialize/Reset bsdconv instance

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

		Return conversion info

	.. py:method:: counter_reset([name])

		Reset counter, if no name supplied, all counters will be reset

/*
 * Copyright (c) 2009-2011 Kuan-Chung Chiu <buganini@gmail.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF MIND, USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <Python.h>
#include <bsdconv.h>

#define IBUFLEN 1024

PyDoc_STRVAR(bsdconv_create_doc,
"create(c)\n\
\n\
Create bsdconv instance.");

static PyObject *
py_bsdconv_create(PyObject *self, PyObject *args)
{
	char *c;
	struct bsdconv_instance *r;

	if (!PyArg_ParseTuple(args, "s", &c))
		return NULL;
	r=bsdconv_create(c);
	if(r==NULL){
		Py_INCREF(Py_None);
		return Py_None;
	}
	return Py_BuildValue("k",r);
}

PyDoc_STRVAR(bsdconv_init_doc,
"init(cd)\n\
\n\
Initialize/Reset bsdconv instance.");

static PyObject *
py_bsdconv_init(PyObject *self, PyObject *args)
{
	unsigned long k;
	struct bsdconv_instance *ins;

	if (!PyArg_ParseTuple(args, "k", &k))
		return NULL;
	ins=(struct bsdconv_instance *) k;
	bsdconv_init(ins);
	Py_INCREF(Py_True);
	return Py_True;
}

PyDoc_STRVAR(bsdconv_insert_phase_doc,
"insert_phase(cd, conversion, phase_type, phasen)\n\
\n\
Insert conversion phase into bsdconv instance.");

static PyObject *
py_bsdconv_insert_phase(PyObject *self, PyObject *args)
{
	unsigned long k;
	struct bsdconv_instance *ins;
	char *s;
	int t;
	int p;

	if (!PyArg_ParseTuple(args, "kzii", &k, &s, &t, &p))
		return NULL;
	ins=(struct bsdconv_instance *) k;
	return Py_BuildValue("i", bsdconv_insert_phase(ins, s, t, p));
}

PyDoc_STRVAR(bsdconv_insert_codec_doc,
"insert_codec(cd, conversion, phasen, codecn)\n\
\n\
Insert conversion codec into bsdconv instance.");

static PyObject *
py_bsdconv_insert_codec(PyObject *self, PyObject *args)
{
	unsigned long k;
	struct bsdconv_instance *ins;
	char *s;
	int p;
	int c;

	if (!PyArg_ParseTuple(args, "kzii", &k, &s, &p, &c))
		return NULL;
	ins=(struct bsdconv_instance *) k;
	return Py_BuildValue("i", bsdconv_insert_codec(ins, s, p, c));
}
PyDoc_STRVAR(bsdconv_replace_phase_doc,
"replace_phase(cd, conversion, phase_type, phasen)\n\
\n\
Replace conversion phase in the bsdconv instance.");

static PyObject *
py_bsdconv_replace_phase(PyObject *self, PyObject *args)
{
	unsigned long k;
	struct bsdconv_instance *ins;
	char *s;
	int t;
	int p;

	if (!PyArg_ParseTuple(args, "kzii", &k, &s, &t, &p))
		return NULL;
	ins=(struct bsdconv_instance *) k;
	return Py_BuildValue("i", bsdconv_replace_phase(ins, s, t, p));
}

PyDoc_STRVAR(bsdconv_replace_codec_doc,
"replace_codec(cd, conversion, phasen, codecn)\n\
\n\
Replace conversion codec in the bsdconv instance.");

static PyObject *
py_bsdconv_replace_codec(PyObject *self, PyObject *args)
{
	unsigned long k;
	struct bsdconv_instance *ins;
	char *s;
	int p;
	int c;

	if (!PyArg_ParseTuple(args, "kzii", &k, &s, &p, &c))
		return NULL;
	ins=(struct bsdconv_instance *) k;
	return Py_BuildValue("i", bsdconv_replace_codec(ins, s, p, c));
}

PyDoc_STRVAR(bsdconv_destroy_doc,
"destroy(p)\n\
\n\
Destroy bsdconv instance.");

static PyObject *
py_bsdconv_destroy(PyObject *self, PyObject *args)
{
	unsigned long k;
	struct bsdconv_instance *r;
	if (!PyArg_ParseTuple(args, "k", &k))
		return NULL;
	r=(struct bsdconv_instance *) k;
	bsdconv_destroy(r);

	Py_INCREF(Py_True);
	return Py_True;
}

PyDoc_STRVAR(bsdconv_conv_doc,
"conv(p,s)\n\
\n\
Perform conversion.");

static PyObject *
py_bsdconv_conv(PyObject *self, PyObject *args)
{
	unsigned long k;
	static PyObject *r;
	struct bsdconv_instance *ins;
	char *s;
	int l;
	if (!PyArg_ParseTuple(args, "kz#", &k,&s,&l))
		return NULL;
	ins=(struct bsdconv_instance *) k;

	bsdconv_init(ins);
	ins->output_mode=BSDCONV_AUTOMALLOC;
	ins->input.data=s;
	ins->input.len=l;
	ins->input.flags=0;
	ins->flush=1;
	bsdconv(ins);

	r=Py_BuildValue("s#",ins->output.data, ins->output.len);
	free(ins->output.data);
	return r;
}

PyDoc_STRVAR(bsdconv_conv_chunk_doc,
"conv_chunk(p,s)\n\
\n\
Perform conversion without initialization and flushing");

static PyObject *
py_bsdconv_conv_chunk(PyObject *self, PyObject *args)
{
	unsigned long k;
	static PyObject *r;
	struct bsdconv_instance *ins;
	char *s;
	int l;
	if (!PyArg_ParseTuple(args, "kz#", &k,&s,&l))
		return NULL;
	ins=(struct bsdconv_instance *) k;

	ins->output_mode=BSDCONV_AUTOMALLOC;
	ins->input.data=s;
	ins->input.len=l;
	ins->input.flags=0;
	bsdconv(ins);

	r=Py_BuildValue("s#",ins->output.data, ins->output.len);
	free(ins->output.data);
	return r;
}

PyDoc_STRVAR(bsdconv_conv_chunk_last_doc,
"conv_chunk_last(p,s)\n\
\n\
Perform conversion without initialization");

static PyObject *
py_bsdconv_conv_chunk_last(PyObject *self, PyObject *args)
{
	unsigned long k;
	static PyObject *r;
	struct bsdconv_instance *ins;
	char *s;
	int l;
	if (!PyArg_ParseTuple(args, "kz#", &k,&s,&l))
		return NULL;
	ins=(struct bsdconv_instance *) k;

	ins->output_mode=BSDCONV_AUTOMALLOC;
	ins->input.data=s;
	ins->input.len=l;
	ins->input.flags=0;
	ins->flush=1;
	bsdconv(ins);

	r=Py_BuildValue("s#",ins->output.data, ins->output.len);
	free(ins->output.data);
	return r;
}

PyDoc_STRVAR(bsdconv_conv_file_doc,
"conv_file(p,s,s)\n\
\n\
Perform conversion with given filename.");

static PyObject *
py_bsdconv_conv_file(PyObject *self, PyObject *args)
{
	unsigned long k;
	struct bsdconv_instance *ins;
	char *s1, *s2;
	FILE *inf, *otf;
	char *in;
	char *tmp;
	int fd;

	if (!PyArg_ParseTuple(args, "kss", &k,&s1,&s2))
		return NULL;
	ins=(struct bsdconv_instance *) k;
	inf=fopen(s1,"r");
	if(!inf){
		Py_INCREF(Py_None);
		return Py_None;
	}
	tmp=malloc(strlen(s2)+8);
	strcpy(tmp, s2);
	strcat(tmp, ".XXXXXX");
	if((fd=mkstemp(tmp))==-1){
		free(tmp);
		Py_INCREF(Py_None);
		return Py_None;
	}
	otf=fdopen(fd,"w");
	if(!otf){
		free(tmp);
		Py_INCREF(Py_None);
		return Py_None;
	}

	bsdconv_init(ins);
	do{
		in=malloc(IBUFLEN);
		ins->input.data=in;
		ins->input.len=fread(in, 1, IBUFLEN, inf);
		ins->input.flags|=F_FREE;
		if(ins->input.len==0){
			ins->flush=1;
		}
		ins->output_mode=BSDCONV_FILE;
		ins->output.data=otf;
		bsdconv(ins);
	}while(ins->flush==0);

	fclose(inf);
	fclose(otf);
	unlink(s2);
	rename(tmp,s2);
	free(tmp);

	Py_INCREF(Py_True);
	return Py_True;
}

static PyObject *
py_bsdconv_info(PyObject *self, PyObject *args)
{
	unsigned long k;
	static PyObject *r;
	struct bsdconv_instance *p;
	if (!PyArg_ParseTuple(args, "k", &k))
		return NULL;
	p=(struct bsdconv_instance *) k;
	r=Py_BuildValue("{sisisi}","ierr",p->ierr,"oerr",p->oerr,"score",p->score);
	return r;
}

static PyObject *
py_bsdconv_error(PyObject *self, PyObject *args)
{
	static PyObject *r;
	char *s;
	s=bsdconv_error();
	r=Py_BuildValue("s",s);
	free(s);
	return r;
}

static PyMethodDef bsdconv_methods[] = {
	{"create",	py_bsdconv_create,	METH_VARARGS,
		PyDoc_STR("create(conversion) -> Create bsdconv instance")},
	{"init",	py_bsdconv_init,	METH_VARARGS,
		PyDoc_STR("init(cd) -> Initialize/Reset bsdconv instance")},
	{"destroy",	py_bsdconv_destroy,	METH_VARARGS,
		PyDoc_STR("destroy(cd) -> Destroy bsdconv instance")},
	{"insert_phase",	py_bsdconv_insert_phase,	METH_VARARGS,
		PyDoc_STR("insert_phase(cd, conversion, phase_type, phasen) -> Insert conversion phase into bsdconv instance")},
	{"insert_codec",	py_bsdconv_insert_codec,	METH_VARARGS,
		PyDoc_STR("insert_phase(cd, conversion, phasen, codecn) -> Insert conversion codec into bsdconv instance")},
	{"replace_phase",	py_bsdconv_insert_phase,	METH_VARARGS,
		PyDoc_STR("replace_phase(cd, conversion, phase_type, phasen) -> Replace conversion phase in the bsdconv instance")},
	{"replace_codec",	py_bsdconv_insert_codec,	METH_VARARGS,
		PyDoc_STR("replace_phase(cd, conversion, phasen, codecn) -> Replace conversion codec in the bsdconv instance")},
	{"conv",	py_bsdconv_conv,	METH_VARARGS,
		PyDoc_STR("conv(cd, s) -> Perform conversion")},
	{"conv_chunk",	py_bsdconv_conv_chunk,	METH_VARARGS,
		PyDoc_STR("conv_chunk(cd, s) -> Perform conversion without initializing and flushing")},
	{"conv_chunk_last",	py_bsdconv_conv_chunk_last,	METH_VARARGS,
		PyDoc_STR("conv_chunk_last(cd, s) -> Perform conversion without initializing")},
	{"conv_file",	py_bsdconv_conv_file,	METH_VARARGS,
		PyDoc_STR("conv_file(cd, from_file, to_file) -> Perform conversion with given filename")},
	{"info",	py_bsdconv_info,	METH_VARARGS,
		PyDoc_STR("info(cd) -> Return conversion info")},
	{"error",	py_bsdconv_error,	METH_VARARGS,
		PyDoc_STR("error() -> Return error message")},
	{NULL,		NULL}		/* sentinel */
};

PyDoc_STRVAR(module_doc,
"BSD licensed charset/encoding converter library");

/* Initialization function for the module (*must* be called initxx) */

PyMODINIT_FUNC
initbsdconv(void)
{
	PyObject *m;
	m = Py_InitModule3("bsdconv", bsdconv_methods, module_doc);
	PyModule_AddIntConstant(m, "FROM", FROM);
	PyModule_AddIntConstant(m, "INTER", INTER);
	PyModule_AddIntConstant(m, "TO", TO);
	if (m == NULL)
		return;
}

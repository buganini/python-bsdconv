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

typedef struct {
	PyObject_HEAD
	struct bsdconv_instance *ins;
} Bsdconv;

PyDoc_STRVAR(bsdconv_init_doc,
"init(cd)\n\
\n\
Initialize/Reset bsdconv instance.");

static PyObject *
py_bsdconv_init(PyObject *self, PyObject *args)
{
	struct bsdconv_instance *ins;

	ins=((Bsdconv *) self)->ins;
	bsdconv_init(ins);
	Py_INCREF(Py_True);
	return Py_True;
}

PyDoc_STRVAR(bsdconv_insert_phase_doc,
"insert_phase(conversion, phase_type, phasen)\n\
\n\
Insert conversion phase into bsdconv instance.");

static PyObject *
py_bsdconv_insert_phase(PyObject *self, PyObject *args)
{
	struct bsdconv_instance *ins;
	char *s;
	int t;
	int p;

	if (!PyArg_ParseTuple(args, "zii", &s, &t, &p))
		return NULL;
	ins=((Bsdconv *) self)->ins;
	return Py_BuildValue("i", bsdconv_insert_phase(ins, s, t, p));
}

PyDoc_STRVAR(bsdconv_insert_codec_doc,
"insert_codec(conversion, phasen, codecn)\n\
\n\
Insert conversion codec into bsdconv instance.");

static PyObject *
py_bsdconv_insert_codec(PyObject *self, PyObject *args)
{
	struct bsdconv_instance *ins;
	char *s;
	int p;
	int c;

	if (!PyArg_ParseTuple(args, "zii", &s, &p, &c))
		return NULL;
	ins=((Bsdconv *) self)->ins;
	return Py_BuildValue("i", bsdconv_insert_codec(ins, s, p, c));
}
PyDoc_STRVAR(bsdconv_replace_phase_doc,
"replace_phase(conversion, phase_type, phasen)\n\
\n\
Replace conversion phase in the bsdconv instance.");

static PyObject *
py_bsdconv_replace_phase(PyObject *self, PyObject *args)
{
	struct bsdconv_instance *ins;
	char *s;
	int t;
	int p;

	if (!PyArg_ParseTuple(args, "zii", &s, &t, &p))
		return NULL;
	ins=((Bsdconv *) self)->ins;
	return Py_BuildValue("i", bsdconv_replace_phase(ins, s, t, p));
}

PyDoc_STRVAR(bsdconv_replace_codec_doc,
"replace_codec(conversion, phasen, codecn)\n\
\n\
Replace conversion codec in the bsdconv instance.");

static PyObject *
py_bsdconv_replace_codec(PyObject *self, PyObject *args)
{
	struct bsdconv_instance *ins;
	char *s;
	int p;
	int c;

	if (!PyArg_ParseTuple(args, "zii", &s, &p, &c))
		return NULL;
	ins=((Bsdconv *) self)->ins;
	return Py_BuildValue("i", bsdconv_replace_codec(ins, s, p, c));
}

PyDoc_STRVAR(bsdconv_dealloc_doc,
"dealloc(p)\n\
\n\
Dealloc bsdconv instance.");

static PyObject *
py_bsdconv_dealloc(PyObject *self)
{
	struct bsdconv_instance *ins;
	ins=((Bsdconv *) self)->ins;
	if(ins!=NULL)
		bsdconv_destroy(ins);
	PyObject_Del(self);
}

PyDoc_STRVAR(bsdconv_conv_doc,
"conv(s)\n\
\n\
Perform conversion.");

static PyObject *
py_bsdconv_conv(PyObject *self, PyObject *args)
{
	static PyObject *r;
	struct bsdconv_instance *ins;
	char *s;
	int l;
	if (!PyArg_ParseTuple(args, "z#", &s,&l))
		return NULL;
	ins=((Bsdconv *) self)->ins;

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
"conv_chunk(s)\n\
\n\
Perform conversion without initialization and flushing");

static PyObject *
py_bsdconv_conv_chunk(PyObject *self, PyObject *args)
{
	static PyObject *r;
	struct bsdconv_instance *ins;
	char *s;
	int l;
	if (!PyArg_ParseTuple(args, "z#", &s,&l))
		return NULL;
	ins=((Bsdconv *) self)->ins;

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
"conv_chunk_last(s)\n\
\n\
Perform conversion without initialization");

static PyObject *
py_bsdconv_conv_chunk_last(PyObject *self, PyObject *args)
{
	static PyObject *r;
	struct bsdconv_instance *ins;
	char *s;
	int l;
	if (!PyArg_ParseTuple(args, "z#", &s, &l))
		return NULL;
	ins=((Bsdconv *) self)->ins;

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
"conv_file(s,s)\n\
\n\
Perform conversion with given filename.");

static PyObject *
py_bsdconv_conv_file(PyObject *self, PyObject *args)
{
	struct bsdconv_instance *ins;
	char *s1, *s2;
	FILE *inf, *otf;
	char *in;
	char *tmp;
	int fd;

	if (!PyArg_ParseTuple(args, "ss", &s1, &s2))
		return NULL;
	ins=((Bsdconv *) self)->ins;
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
	static PyObject *r;
	struct bsdconv_instance *ins;
	ins=((Bsdconv *) self)->ins;
	r=Py_BuildValue("{sisisi}","ierr",ins->ierr,"oerr",ins->oerr,"score",ins->score);
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

int
py_bsdconv_valid(PyObject *self, PyObject *args)
{
	struct bsdconv_instance *ins;
	ins=((Bsdconv *) self)->ins;
	
	if(ins!=NULL)
		return 1;
	else
		return 0;
}

static PyMethodDef Bsdconv_methods[] = {
	{"init",	py_bsdconv_init,	METH_VARARGS,
		PyDoc_STR("init(cd) -> Initialize/Reset bsdconv instance")},
	{"insert_phase",	py_bsdconv_insert_phase,	METH_VARARGS,
		PyDoc_STR("insert_phase(conversion, phase_type, phasen) -> Insert conversion phase into bsdconv instance")},
	{"insert_codec",	py_bsdconv_insert_codec,	METH_VARARGS,
		PyDoc_STR("insert_phase(conversion, phasen, codecn) -> Insert conversion codec into bsdconv instance")},
	{"replace_phase",	py_bsdconv_insert_phase,	METH_VARARGS,
		PyDoc_STR("replace_phase(conversion, phase_type, phasen) -> Replace conversion phase in the bsdconv instance")},
	{"replace_codec",	py_bsdconv_insert_codec,	METH_VARARGS,
		PyDoc_STR("replace_phase(conversion, phasen, codecn) -> Replace conversion codec in the bsdconv instance")},
	{"conv",	py_bsdconv_conv,	METH_VARARGS,
		PyDoc_STR("conv(s) -> Perform conversion")},
	{"conv_chunk",	py_bsdconv_conv_chunk,	METH_VARARGS,
		PyDoc_STR("conv_chunk(s) -> Perform conversion without initializing and flushing")},
	{"conv_chunk_last",	py_bsdconv_conv_chunk_last,	METH_VARARGS,
		PyDoc_STR("conv_chunk_last(s) -> Perform conversion without initializing")},
	{"conv_file",	py_bsdconv_conv_file,	METH_VARARGS,
		PyDoc_STR("conv_file(from_file, to_file) -> Perform conversion with given filename")},
	{"info",	py_bsdconv_info,	METH_VARARGS,
		PyDoc_STR("info(cd) -> Return conversion info")},
	{NULL,		NULL}		/* sentinel */
};

static PyNumberMethods Bsdconv_number_methods = {
#if PY_MAJOR_VERSION < 3
	.nb_nonzero = (inquiry) py_bsdconv_valid
#else
	.nb_bool = (inquiry) py_bsdconv_valid
#endif
};

#if PY_MAJOR_VERSION < 3
static PyObject *
py_bsdconv_getattr(PyObject *self, char *attrname)
{
	return Py_FindMethod(Bsdconv_methods, self, attrname);
}
#endif

static PyObject *
py_bsdconv_repr(PyObject *self, char *attrname)
{
	static PyObject *r;
	char *s;
	int len=32;
	struct bsdconv_instance *ins;
	ins=((Bsdconv *) self)->ins;
	if(ins==NULL){
		Py_INCREF(Py_None);
		return Py_None;
	}

	s=bsdconv_pack(ins);
	len+=strlen(s);
	char buf[len];
	sprintf(buf, "Bsdconv(\"%s\") at %p", s, ins);
	r=Py_BuildValue("s", buf);
	free(s);
	return r;
}

static PyTypeObject Bsdconv_Type = {
#if PY_MAJOR_VERSION < 3
	PyObject_HEAD_INIT(NULL)
#else
	PyVarObject_HEAD_INIT(NULL, 0)
#endif
	.tp_name="Bsdconv",
	.tp_basicsize=sizeof(Bsdconv),
	.tp_dealloc = (destructor)py_bsdconv_dealloc,
#if PY_MAJOR_VERSION < 3
	.tp_getattr = (getattrfunc)py_bsdconv_getattr,
#endif
	.tp_repr = (reprfunc)py_bsdconv_repr,
	.tp_methods = Bsdconv_methods,
	.tp_as_number = &Bsdconv_number_methods
};

PyDoc_STRVAR(bsdconv_create_doc,
"new(c)\n\
\n\
Create bsdconv instance.");

static PyObject *
py_bsdconv_NEW(const char *c)
{
	Bsdconv *object = NULL;
	object = PyObject_NEW(Bsdconv, &Bsdconv_Type);
	object->ins=NULL;
	if (object != NULL)
		object->ins = bsdconv_create(c);
	return (PyObject *)object;
}

static PyObject *
py_bsdconv_new(PyObject *self, PyObject *args)
{
	PyObject *ret = NULL;
	char *c;
	if (PyArg_ParseTuple(args, "s", &c))
		ret=py_bsdconv_NEW(c);
	else
		ret=py_bsdconv_NEW("");
	return ret;
}

PyDoc_STRVAR(bsdconv_codecs_list_doc,
"codecs_list(type,codec)\n\
\n\
list codecs.");

static PyObject *
py_bsdconv_codecs_list(PyObject *self, PyObject *args)
{
	PyObject *ret=PyDict_New();
	PyObject *tmp;
	int i;
	char *type[]={"from","inter","to"};
	char **list;
	char **p;
	list=bsdconv_codecs_list();
	p=list;
	for(i=0;i<3;++i){
		tmp=PyList_New(0);
		while(*p!=NULL){
			PyList_Append(tmp, Py_BuildValue("s", *p));
			free(*p);
			p+=1;
		}
		PyDict_SetItemString(ret, type[i], tmp);
		p+=1;
	}
	free(list);
	return ret;
}

PyDoc_STRVAR(bsdconv_codec_check_doc,
"codec_check(type, codec)\n\
\n\
check if a codec is available.");

static PyObject *
py_bsdconv_codec_check(PyObject *self, PyObject *args)
{
	PyObject *r;
	struct bsdconv_instance *ins;
	char *s;
	int type;
	if (!PyArg_ParseTuple(args, "iz", &type, &s))
		return NULL;
	if(bsdconv_codec_check(type, s))
		r=Py_True;
	else
		r=Py_False;
	Py_INCREF(r);
	return r;
}

static PyMethodDef module_methods[] = {
	{"error",	py_bsdconv_error,	METH_VARARGS,
		PyDoc_STR("error() -> Return error message")},
	{"codecs_list",	py_bsdconv_codecs_list,	METH_VARARGS,
		PyDoc_STR("codecs_list() -> list codecs")},
	{"codec_check",	py_bsdconv_codec_check,	METH_VARARGS,
		PyDoc_STR("codec_check(type, codec) -> check/solve codec and codec alias")},
	{NULL,		NULL}		/* sentinel */
};
#if PY_MAJOR_VERSION >= 3
static PyModuleDef Bsdconv_Module = {
	PyModuleDef_HEAD_INIT,
	"bsdconv",
	"BSD licensed charset/encoding converter library",
	-1,
	module_methods,
	NULL, NULL, NULL, NULL
};
#endif

PyDoc_STRVAR(module_doc,
"BSD licensed charset/encoding converter library");

/* Initialization function for the module (*must* be called initxx) */

PyMODINIT_FUNC
#if PY_MAJOR_VERSION < 3
initbsdconv(void)
#else
PyInit_bsdconv(void)
#endif
{
	PyObject *m;
	Bsdconv_Type.tp_new = (newfunc)py_bsdconv_new;
#if PY_MAJOR_VERSION < 3
	if (PyType_Ready(&Bsdconv_Type) < 0)
		return;
	m = Py_InitModule3("bsdconv", module_methods, module_doc);
	if (m == NULL)
		return;
#else
	if (PyType_Ready(&Bsdconv_Type) < 0){
		return NULL;
	}
	m = PyModule_Create(&Bsdconv_Module);
	if (m == NULL){
		return NULL;
	}
#endif
	Py_INCREF(&Bsdconv_Type);
	PyModule_AddObject(m, "Bsdconv", (PyObject *)&Bsdconv_Type);
	PyModule_AddIntConstant(m, "FROM", FROM);
	PyModule_AddIntConstant(m, "INTER", INTER);
	PyModule_AddIntConstant(m, "TO", TO);
#if PY_MAJOR_VERSION >= 3
	return m;
#endif
}

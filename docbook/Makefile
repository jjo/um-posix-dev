#
#	PnP Makefile para SGML
#
# Hacer 'just:
# 	make all-dvi all-txt all-html
#
TEX=latex < /dev/null
DVI2TTY=/usr/local/bin/dvi2tty
DVI2PS=dvips
LATEX2RTF=latex2rtf
SGML_OPT=--charset=latin --language=es
SGML2TXT=sgml2txt $(SGML_OPT)
JADETEX=jadetex
DPATH1:=/usr/share/sgml/docbook/dsssl-stylesheets
#DPATH2:=/usr/lib/sgml/stylesheets/nwalsh-modular
DPATH_PRINT:=$(DPATH1)/print #:$(DPATH2)/print
DPATH_HTML:=$(DPATH1)/html #:$(DPATH2)/html
CATALOG:=/usr/share/sgml/docbook/sgml-dtd-4.1/catalog
#CATALOG:=/usr/share/sgml/docbook/xml-dtd-4.1.2/catalog
OPENJADE=jade -c $(CATALOG)

#JADEPRINT=$(OPENJADE) -D $(DPATH_PRINT) -d /usr/share/sgml/docbook/dsssl-stylesheets-cygnus/stylesheets/cygnus-both.dsl\#print
JADEPRINT=$(OPENJADE) -D $(DPATH_PRINT) -d d1.dsl

#JADEHTML=$(OPENJADE) -D $(DPATH_HTML) -t sgml -ihtml -d /usr/share/sgml/docbook/dsssl-stylesheets-cygnus/stylesheets/cygnus-both.dsl\#html
JADEHTML=$(OPENJADE) -D $(DPATH_HTML) -t sgml -i html -d d1.dsl

#SGML2HTML=sgml2html $(SGML_OPT) -s 0

all: Informatica_III.dvi

%.html : %.sgml *.sgml
	#test -d $* || mkdir $*
	#cd $* && $(JADEHTML) ../$<
	test -d html-out || mkdir html-out
	$(JADEHTML) -o $@ $<
	-mv $*.css $*

%.ok: %.sgml
	nsgmls -s -C $(CATALOG) $?
%.dvi : %.tex *.sgml
	$(JADETEX) $<
%.tex : %.sgml *.sgml
	$(JADEPRINT) -t tex $<

%.txt : %.sgml
	$(SGML2TXT) $< 


.PRECIOUS: %.dvi %.ps

%.ps : %.dvi
	$(DVI2PS) -o$@ $<

%.rtf : %.tex
	$(LATEX2RTF) -o$@ $<

%.out : %.dvi
	$(DVI2TTY) $<

lpr-% : %.ps
	lpr $<

#FILES_TEX=$(wildcard *.tex)
FILES_RTF=$(patsubst %.tex,%.rtf,$(FILES_TEX))
FILES_PS=$(patsubst %.dvi,%.ps,$(FILES_DVI))
LPR_FILES=$(patsubst %.ps,lpr-%, $(FILES_PS))

# TeX
#FILES_DVI=$(patsubst %.tex,%.dvi,$(FILES_TEX))

# SGML
FILES_SGML=$(wildcard *.sgml)
FILES_DVI=$(patsubst %.sgml,%.dvi,$(FILES_SGML))
FILES_TEX=$(patsubst %.sgml,%.tex,$(FILES_SGML))
FILES_HTML=$(patsubst %.sgml,%.html,$(FILES_SGML))
FILES_CSS=$(patsubst %.sgml,%.css,$(FILES_SGML))
FILES_TXT=$(patsubst %.sgml,%.txt,$(FILES_SGML))
FILES_FOT=$(patsubst %.sgml,%.fot,$(FILES_SGML))

all-ps : $(FILES_PS)
all-html : $(FILES_HTML)
all-txt : $(FILES_TXT)
all-rtf : $(FILES_RTF)
all-lpr : $(LPR_FILES)
all-dvi : $(FILES_DVI)
all-tex : $(FILES_TEX)



clean-dvi:
	rm -f *.con *.aux *.log *~ core

moreclean: clean-dvi
	rm -f $(FILES_TXT) $(FILES_DVI) $(FILES_TEX) $(FILES_HTML) $(FILES_CSS) $(FILES_PS) $(FILES_FOT) *.htm


NAME    = oz-complexity

TEX     = $(NAME).tex
PS      = $(NAME).ps
DVI     = $(NAME).dvi
TAR     = $(NAME).tar
GZ      = $(TAR).gz
ERRS    = $(NAME).ok-errs
DVIPS   = dvips
FLOPPY  = /dev/fd0
FIGS    = figures

RELEASE = $(SRC) Makefile references.bib old-text.TEX iospress.sty epsf.sty $(ERRS) \
          bin/README bin/graph bin/make-sampled-graph bin/process-abundance \
          bin/cluster-cluster bin/make-graph bin/preston bin/sample \
          $(FIGS)/agent-agent.ps $(FIGS)/agent-evolution.ps \
          $(FIGS)/example-agent.ps \
          $(FIGS)/preston.ps $(FIGS)/preston.1.ps $(FIGS)/preston.3.ps \
          $(FIGS)/preston.5.ps $(FIGS)/preston.7.ps $(FIGS)/preston.9.ps \
          $(FIGS)/grouped-100.ps $(FIGS)/grouped-200.ps $(FIGS)/grouped-300.ps \
          $(FIGS)/preston-effect-of-grouping.ps 

$(PS) : $(DVI)
	$(DVIPS) -R -o $(PS) $(DVI)

$(DVI): $(TEX)
	latex $(TEX) && bibtex $(NAME) && latex $(TEX)

spell:
	@spell $(TEX) | sort -u > /tmp/fred
	@if [ ! -f $(ERRS) ]; then touch $(ERRS); fi
	@comm -13 $(ERRS) /tmp/fred | sort -f
	@rm -f /tmp/fred

spellok:
	@if [ ! -f $(ERRS) ]; then touch $(ERRS); fi
	@(cat $(ERRS); spell $(TEX)) | sort -u > /tmp/fred && mv /tmp/fred $(ERRS)

print: $(PS)
	lpr $(PS)

clean:
	rm -f *.toc *.log *.aux *.idx *~ *.dvi

clobber: clean
	rm -f $(PS)

.tar: $(RELEASE)
	tar cfv $(TAR) $(RELEASE) echo-output
	gzip -f $(TAR)
	touch .tar

floppy: .tar
	@ls -l $(GZ)
	tar cfv $(FLOPPY) $(GZ)
	@echo
	@echo "Reading from floppy for verification."
	@echo
	tar tfv $(FLOPPY)
	eject

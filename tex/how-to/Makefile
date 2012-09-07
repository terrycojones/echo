TEX = how-to.tex

how-to.ps : how-to.dvi
	dvi2ps -r how-to.dvi > how-to.ps

how-to.dvi: how-to.tex how-to.index
	-latex how-to.tex
	-latex how-to.tex

how-to.index: how-to.tex
	-do-index

spell:
	@spell how-to.tex | sort -f -u > /tmp/fred
	@comm -13 ok-errs /tmp/fred
	@rm -f /tmp/fred

print: how-to.ps
	lpr how-to.ps

spellok:
	@(cat ok-errs; spell how-to.tex) | sort -f -u > /tmp/fred && mv /tmp/fred ok-errs

clean:
	rm -f *.toc *.log *.aux *.idx *~ *.dvi

clobber: clean
	rm -f *.ps *.index

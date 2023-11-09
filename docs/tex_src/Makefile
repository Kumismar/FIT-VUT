PROJ := manual

$(PROJ).pdf: $(PROJ).ps
	ps2pdf $(PROJ).ps

$(PROJ).ps: $(PROJ).dvi
	dvips -t a4 $(PROJ).dvi

$(PROJ).dvi: $(PROJ).tex
	latex $(PROJ).tex
	latex $(PROJ).tex
pdflatex: $(PROJ).tex
	pdflatex $(PROJ).tex
	
clean:
	rm -f $(PROJ).aux
	rm -f $(PROJ).out
	rm -f $(PROJ).dvi
	rm -f $(PROJ).log
	rm -f $(PROJ).ps
	rm -f $(PROJ).synctex.gz
	rm -f $(PROJ).fls
	rm -f $(PROJ).fdb_latexmk
PDF = index.pdf
MAIN = main
FLAGS = -shell-escape
CC = pdflatex
SHIT = *.aux *.log *.out *.toc *.bbl *.blg

all: s

s: $(MAIN).tex *.tex themes/*.tex
	make resurrect
	-$(CC) $(FLAGS) $< #> /dev/null
	-mv $(MAIN).pdf $(PDF)
	make moveout

toc : $(MAIN).tex *.tex themes/*.tex
	make resurrect
	-$(CC) $(FLAGS) $< #> /dev/null
	-$(CC) $(FLAGS) $< > /dev/null
	-mv $(MAIN).pdf $(PDF)
	make moveout

bib: $(MAIN).tex *.tex themes/*.tex
	make resurrect
	-$(CC) $(FLAGS) $<  #> /dev/null
	-bibtex $(MAIN).aux > /dev/null
	-$(CC) $(FLAGS) $<  > /dev/null
	-$(CC) $(FLAGS) $<  > /dev/null
	-mv $(MAIN).pdf $(PDF)
	make moveout

moveout:
	-mkdir -p out > /dev/null
	-mkdir -p out/themes > /dev/null
	-mv $(SHIT) out > /dev/null
	-cd themes && mv *.aux ../out/themes > /dev/null

resurrect:
	-mv out/themes/* themes > /dev/null
	-mv out/* . > /dev/null

clean:
	-rm  $(SHIT) > /dev/null
	-cd themes && rm $(SHIT) > /dev/null
	rm -rf out

.PHONY: all, clean, bib, moveout, resurrect

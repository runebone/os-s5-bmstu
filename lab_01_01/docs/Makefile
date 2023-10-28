PDF := index.pdf
MAIN := main

.PHONY: all, pdf
all: pdf move

pdf: $(MAIN).tex *.tex
	# xelatex $<
	pdflatex -shell-escape $^
	mv $(MAIN).pdf $(PDF)

.PHONY: move
move: $(PDF)
	mkdir -p ../out
	-mv *.aux *.log *.out *.toc *.bbl *.blg $(MAIN).bcf $(MAIN).run.xml ../out
	-mv *.pdf ..

.PHONY: clean
clean:
	-rm *.aux *.log *.out *.toc *.bbl *.blg $(MAIN).bcf $(MAIN).run.xml
	-rm -rf ../out

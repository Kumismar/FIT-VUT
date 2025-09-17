MAIN_FILE = dokumentace

PDFLATEX = pdflatex

AUX_DIR = aux_files

PDFLATEX_FLAGS = -output-directory=$(AUX_DIR)

all: $(MAIN_FILE).pdf

$(MAIN_FILE).pdf: $(MAIN_FILE).tex
	@mkdir -p $(AUX_DIR)
	$(PDFLATEX) $(PDFLATEX_FLAGS) $<
	$(PDFLATEX) $(PDFLATEX_FLAGS) $<
	$(PDFLATEX) $(PDFLATEX_FLAGS) $< 
	$(PDFLATEX) $(PDFLATEX_FLAGS) $<
	@mv $(AUX_DIR)/$(MAIN_FILE).pdf .

clean:
	@rm -rf $(AUX_DIR)
	@rm -f $(MAIN_FILE).pdf

.PHONY: all clean 

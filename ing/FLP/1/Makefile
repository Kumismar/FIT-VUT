SRCDIR = src
BUILDDIR = build

SOURCE_FILES = $(wildcard $(SRCDIR)/*.hs)
EXECUTABLE = flp-fun

GHC := ghc
GHCFLAGS := -Wall --make -i$(SRCDIR) -hidir $(BUILDDIR) -hiedir $(BUILDDIR) -odir $(BUILDDIR)

all: $(EXECUTABLE)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(EXECUTABLE): $(SOURCE_FILES) | $(BUILDDIR)
	$(GHC) $(GHCFLAGS) $(SRCDIR)/Main.hs -o $@
	cp $(EXECUTABLE) public/

test: $(EXECUTABLE)
	cd public/ && python test_flp.py

clean:
	rm -rf $(BUILDDIR)
	rm -f $(EXECUTABLE)

PROGNAME=docker-enter

BINDIR?=/usr/bin

$(PROGNAME):	docker-enter.c
	$(CC) $< -o $@

clean:
	rm -f $(PROGNAME)

install:
	install -d $(DESTDIR)/$(BINDIR)
	install $(PROGNAME) -t $(DESTDIR)/$(BINDIR) --mode=u=rwx,go=x

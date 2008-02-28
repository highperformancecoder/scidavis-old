BASE=scidavis-manual-0.1_`date --iso`
html-en:
	docbook2html --dsl scidavis.dsl docbook-en/index.docbook

html-packages:
	rm -rf manual
	cp -r html manual
	find manual -depth -name .svn -exec rm -rf \{\} \;
	tar cjf ${BASE}.tar.bz2 manual
	zip -9r ${BASE}.zip manual

pdf-en:
	dblatex -o ${BASE}.pdf -c dblatex.conf -p dblatex-param.xsl docbook-en/index.docbook

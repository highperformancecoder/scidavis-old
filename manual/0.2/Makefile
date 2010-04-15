BASE=scidavis-manual-0.2_`date --iso`

html-en:
	xsltproc scidavis_html.xsl docbook-en/index.docbook

html-packages: html-en
	rm -rf manual
	cp -r html manual
	find manual -depth -name .svn -exec rm -rf \{\} \;
	tar cjf ${BASE}.tar.bz2 manual
	zip -9r ${BASE}.zip manual

pdf-en:
	dblatex -o ${BASE}.pdf -c dblatex.conf -p dblatex-param.xsl docbook-en/index.docbook

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>
<xsl:import href="/usr/share/xml/docbook/xsl-stylesheets-1.75.2/html/chunk.xsl"/>
<xsl:import href="docbook2adp.xsl"/>

<xsl:param name="chunker.output.encoding">UTF-8</xsl:param>
<xsl:param name="chunker.output.indent">yes</xsl:param>

<xsl:param name="html.stylesheet">scidavis.css</xsl:param>

<xsl:param name="css.decoration" select="1"></xsl:param>

<xsl:param name="use.id.as.filename" select="'1'"></xsl:param>
<xsl:param name="base.dir" select="'html/'"></xsl:param>

<xsl:param name="adp.file">scidavis.adp</xsl:param>
<xsl:param name="adp.name">scidavis_manual_en</xsl:param>
<xsl:param name="adp.appicon">./common/scidavis_logo.png</xsl:param>
<!--
		control of the index
-->
<xsl:param name="generate.index" select="1"></xsl:param>
<xsl:param name="index.method" select="'basic'"></xsl:param>
<xsl:param name="index.prefer.titleabbrev" select="1"></xsl:param>
<xsl:param name="index.term.separator" select="''"></xsl:param>
<xsl:param name="index.on.type" select="0"></xsl:param>
<!-- 
		control of table of content
-->
<!--		main TOC is in title page if =0 -->
<xsl:param name="chunk.tocs.and.lots" select="0"></xsl:param>
<!--		TOC, LOT and LOF are on separate pages if =1 -->
<xsl:param name="chunk.separate.lots" select="1"></xsl:param>

<xsl:param name="toc.section.depth">2</xsl:param>
<!--		generate secondary TOC for main sections -->
<xsl:param name="generate.section.toc.level" select="1"></xsl:param>
<!--		max depth for a TOC -->
<xsl:param name="toc.max.depth">2</xsl:param>
<!--		elements which should appear in TOC section -->
<xsl:param name="generate.toc">
book      toc,title,figures,tables
chapter   toc,title
appendix  toc,title
</xsl:param>
<!--		should the faq appear in the toc -->
<xsl:param name="qanda.in.toc" select="1"></xsl:param>
<xsl:param name="qanda.nested.in.toc" select="0"></xsl:param>

<!--		control of Q & A set				-->
<xsl:param name="qanda.defaultlabel">none</xsl:param>
<xsl:param name="qanda.inherit.numeration" select="0"></xsl:param>

<xsl:param name="make.year.ranges" select="0"></xsl:param>

</xsl:stylesheet>

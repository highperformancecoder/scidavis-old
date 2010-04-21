<!--
This stylesheets is intended to be used together with the
Docbook -> HTML stylesheet html/chunk.xsl available from
http://docbook.sourceforge.net. It adds support for Qt Assistant
Document Profile (.adp) files. You need to import both chunk.xsl
and docbook2adp.xsl in your stylesheet, where you may also adjust
parameters of both (see xsl:param elements below).

Copyright (C) 2010 Knut Franke, knut.franke*gmx.de

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the ``Software''), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

Except as contained in this notice, the names of individuals
credited with contribution to this software shall not be used in
advertising or otherwise to promote the sale, use or other
dealings in this Software without prior written authorization
from the individuals in question.

Warranty
========
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT.  IN NO EVENT SHALL NORMAN WALSH OR ANY OTHER
CONTRIBUTOR BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
-->

<xsl:stylesheet  version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform' >

	<xsl:param name="adp.file">output.adp</xsl:param>
	<xsl:param name="adp.name">output</xsl:param>
	<xsl:param name="adp.appicon"/>

	<xsl:template match="/">
		<xsl:apply-templates select="/" mode="collect.targets"/>
		<xsl:apply-templates select="/" mode="process.root"/>
		<xsl:call-template name="write.chunk">
			<xsl:with-param name="filename"><xsl:value-of select="$base.dir"/><xsl:value-of select="$adp.file"/></xsl:with-param>
			<xsl:with-param name="content"><xsl:apply-templates select="/" mode="adp"/></xsl:with-param>
		</xsl:call-template>
	</xsl:template>

	<xsl:template match="/" mode="adp">
		<assistantconfig version="3.2.0">
			<profile>
				<property name='name'><xsl:value-of select="$adp.name"/></property>
				<property name='title'><xsl:value-of select="//bookinfo/title"/></property>
				<property name='applicationicon'><xsl:value-of select="$adp.appicon"/></property>
				<property name='startpage'>./index.html</property>
				<property name='aboutmenutext'><xsl:value-of select="//bookinfo/title"/></property>
				<property name='abouturl'>./index.html</property>
			</profile>
			<DCF ref="index.html"><xsl:attribute name="title"><xsl:value-of select="//bookinfo/title"/></xsl:attribute>
				<xsl:for-each select="//chapter"><xsl:call-template name="chapter"/></xsl:for-each>
			</DCF>
		</assistantconfig>
	</xsl:template>

	<xsl:template name="chapter" match="chapter" mode="adp">
		<section>
			<xsl:attribute name="ref"><xsl:value-of select="@id"/>.html#<xsl:value-of select="@id"/></xsl:attribute>
			<xsl:attribute name="title"><xsl:value-of select="title"/></xsl:attribute>
			<xsl:for-each select="descendant::indexterm">
				<xsl:if test="not(ancestor::sect1)">
					<keyword>
						<xsl:attribute name="ref"><xsl:value-of select="ancestor::sect1/@id"/>.html#<xsl:call-template name="object.id"/></xsl:attribute>
						<xsl:value-of select="primary"/>
					</keyword>
				</xsl:if>
			</xsl:for-each>
			<xsl:for-each select="sect1"><xsl:call-template name="sect1"/></xsl:for-each>
		</section>
	</xsl:template>

	<xsl:template name="sect1" match="sect1" mode="adp">
		<section>
			<xsl:attribute name="ref">
				<xsl:choose>
					<xsl:when test="preceding-sibling::sect1"><xsl:value-of select="@id"/>.html#<xsl:value-of select="@id"/></xsl:when>
					<xsl:otherwise><xsl:value-of select="../@id"/>.html#<xsl:value-of select="@id"/></xsl:otherwise>
				</xsl:choose>
			</xsl:attribute>
			<xsl:attribute name="title"><xsl:value-of select="title"/></xsl:attribute>
			<xsl:for-each select="descendant::indexterm">
				<keyword>
					<xsl:attribute name="ref">
						<xsl:variable name="ref.page">
							<xsl:choose>
								<xsl:when test="ancestor::sect1/preceding-sibling::sect1">
									<xsl:value-of select="ancestor::sect1/@id"/>.html
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="ancestor::sect1/../@id"/>.html
								</xsl:otherwise>
							</xsl:choose>
						</xsl:variable>
						<xsl:value-of select="normalize-space($ref.page)"/>#<xsl:call-template name="object.id"/></xsl:attribute>
					<xsl:value-of select="primary"/>
					<xsl:if test="secondary">, <xsl:value-of select="secondary"/></xsl:if>
					<xsl:if test="tertiary">, <xsl:value-of select="tertiary"/></xsl:if>
				</keyword>
			</xsl:for-each>
		</section>
	</xsl:template>

</xsl:stylesheet>

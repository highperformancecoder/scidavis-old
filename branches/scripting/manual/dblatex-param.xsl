<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>

<!-- 
     réduit la taille des images d'un facteur 0.4 pour le fichier PDF
	par rapport à la taille utilisée pour les fichiers html 
-->
<xsl:import href="qtiplot-style.xsl"/>

<xsl:param name="imagedata.default.scale">scale=0.4</xsl:param>

<xsl:param name="qandaset.defaultlabel" value="number"></xsl:param>

</xsl:stylesheet>

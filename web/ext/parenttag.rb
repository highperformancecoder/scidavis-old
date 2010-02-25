# Outputs the title of the current node's ancestor, specified by level
class ParentTag
	include Webgen::Tag::Base

	def call(tag, body, context)
		level = param('parenttag.level')
		i = context.content_node # the current node
		return "" if i.level <= level
		while i.level > level do i = i.parent end
		lang_node = i.in_lang( context.content_node.lang )
		i.cn + param('parenttag.postfix')
	end
end

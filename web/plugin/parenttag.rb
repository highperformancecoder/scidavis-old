class ParentTag < Tags::DefaultTag
	infos( :name => 'SciDAVis/ParentTag',
			 :summary => "Outputs the title of a current node's parent, specified by level")

	param 'level', 1, 'the level of the desired parent node'
	param 'postfix', "", 'string to append to the output, e.g. ": " (left out if there\'s no matching parent)'

	register_tag 'parent'

	def process_tag(tag, chain)
		level = param('level')
		i = chain.last # the current node
		return "" if i.level <= level
		while i.level > level do i = i.parent end
		lang_node = i.node_for_lang( chain.last['lang'] )
		(lang_node['directoryName'] || i['title']) + param('postfix')
	end
end

#
#--
#
# $Id: meta.rb 462 2006-08-19 15:23:42Z thomas $
#
# webgen: template based static website generator
# Copyright (C) 2004 Thomas Leitner
#
# This program is free software; you can redistribute it and/or modify it under the terms of the GNU
# General Public License as published by the Free Software Foundation; either version 2 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with this program; if not,
# write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
#
#++
#

  # Substitutes the tag with an actual content block.
  class HighlightBlockTag < Tags::DefaultTag

    infos( :name => 'SciDAVis/HighlightBlock',
           :author => Webgen::AUTHOR,
           :summary => "Returns the rendered content of a page file block"
           )

    param 'blockName', 'content', 'The name of the block which should be rendered.'
    set_mandatory 'blockName', true
	 param 'highlight', nil, 'Name of language that should be used for syntax highlighting of ' +
		'the content of the file. If set to nil, no highlighting is performed. '

    register_tag 'hlblock'

    def process_tag( tag, chain )
      block_node = (chain.length > 1 ? chain[1] : chain[0])
      block_name = param( 'blockName' )
      if block_node.node_info[:pagedata].blocks.has_key?( block_name )
        if block_node['useERB']
          node = chain.last
          ref_node = block_node
          begin
            content = block_node.node_info[:pagedata].blocks[block_name].render_with_erb( binding )
          rescue
            log(:error) { "Error while running ERB on <#{block_node.node_info[:src]}> (block '#{block_name}'): #{$!.message}" }
            content = ''
          end
        else
          content = block_node.node_info[:pagedata].blocks[block_name].content
        end
		  if !param( 'highlight' ).nil?
			  content = @plugin_manager['Misc/SyntaxHighlighter'].highlight( content, param( 'highlight' ) )
		  end
        [content, (chain[1..-1].empty? ? chain : chain[1..-1]) ]
      else
        log(:error) { "Node <#{block_node.node_info[:src]}> does not contain a block called '#{block_name}'" }
        ''
      end
    end

  end


DOC=$(PWD)
TMP=/tmp
WIKI=wiki
VIMWIKI=$(PWD)/../Externals/vimwiki

all doc: clean
	@cp -r $(DOC)/$(WIKI) $(TMP)/$(WIKI)
	@find $(TMP)/$(WIKI) -type f -iname "*.wiki" | xargs -i@ sh -c "cat @ | grep \"?include\" | sed \"s/[^\\\"]*\\\"\(.*\)\\\".*/\1/\" | xargs -i% sed -i\".bak\" -e\"/<?include\s*=\s*".*"/r $(DOC)/%\" -e\"/<?include\s*=\s*".*"/d\" @"
	@touch $(VIMWIKI)/syntax/syntax.vim
	@VIMRUNTIME="$(VIMWIKI)" vim -n --noplugins -u NONE\
		-c "set nocompatible | filetype plugin on | syntax on"\
		-c "let g:vimwiki_list = [{'path': '$(TMP)/$(WIKI)', 'path_html': '$(DOC)/html/wiki', 'template_path': '$(TMP)/$(WIKI)/templates/', 'template_default': 'wiki', 'template_ext': '.tpl'}]"\
		-c "source $(VIMWIKI)/plugin/vimwiki.vim"\
		-c "source $(VIMWIKI)/ftplugin/vimwiki.vim"\
		-c "source $(VIMWIKI)/syntax/vimwiki.vim"\
		-c "VimwikiAll2HTML"\
		-c "quit"\
		$(TMP)/$(WIKI)/index.wiki
	@rm -f $(VIMWIKI)/syntax/syntax.vim

clean:
	@rm -rf $(TMP)/$(WIKI)


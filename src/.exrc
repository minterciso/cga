if &cp | set nocp | endif
map Q gq
let s:cpo_save=&cpo
set cpo&vim
nmap gx <Plug>NetrwBrowseX
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#NetrwBrowseX(expand("<cWORD>"),0)
inoremap  u
let &cpo=s:cpo_save
unlet s:cpo_save
set backspace=indent,eol,start
set backup
set backupskip=/tmp/*,/var/spool/cron/*
set expandtab
set history=50
set hlsearch
set incsearch
set mouse=a
set ruler
set shiftwidth=2
set showcmd
set smartindent
set softtabstop=2
set window=30
" vim: set ft=vim :

# RPM spec file for konoha					         -*-rpm-spec-*-
#
# Copyright (c) 2008-2009 Konoha Software Foundation
#

%define name    konoha
%define version 0.70
%define release 1
%define encoding utf8
%define threads  pthreads


Name:           %{name}
Version:        %{version}
Release:        %{release}
License:        LGPL
Group:          Development/Languages
URL:            http://konoha.sourceforge.jp/
Source0:        http://haskell.org/ghc/dist/%{version}/ghc-%{version}-src.tar.bz2
Source1:        http://haskell.org/ghc/dist/%{version}/ghc-%{version}-src-extralibs.tar.bz2
Packager:       Masahiro Ide <imasahiro@>
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
PreReq:         update-alternatives
Requires:       readline
Provides:       haskell
Summary:        konoha - a newly designed object-oriented scripting language

%description
Konoha is a newly designed object-oriented scripting language, providing hybrid 
type system for static/dynamic typing, mappable class, and various experimental
features.

Haskell is the standard lazy purely functional programming language.
The current language version is Haskell 98, agreed in December 1998,
with a revised version published in January 2003.

%prep
%setup

%build
./configure --prefix=/usr --mandir='${prefix}/share/man' --infodir='${prefix}/share/info' 
make

%install
# These dirs are not cleared after rpm -ba --clean.   To ensure clean
# install, we remove them.
mkdir -p ${RPM_BUILD_ROOT}/usr
make DESTDIR=${RPM_BUILD_ROOT}/ install-pkg
make DESTDIR=${RPM_BUILD_ROOT}/ install-doc

%clean

%post


%files -f rpm-noprof-lib-files
%defattr(-,root,root)
%doc LICENSE
%doc README
%{_mandir}/man1/konoha.1
%{_prefix}/bin/konoha
%{_prefix}/bin/klip

%files prof -f rpm-prof-lib-files
%defattr(-,root,root)

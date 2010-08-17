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
Source0:        http://sourceforge.jp/projects/konoha/downloads/40309/konoha-%{version}.tar.gz
Packager:       Masahiro Ide <imasahiro@gmail.com>
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
PreReq:         update-alternatives
Requires:       readline
Summary:        konoha - a newly designed object-oriented scripting language
%description
Konoha is a newly designed object-oriented scripting language, providing hybrid 
type system for static/dynamic typing, mappable class, and various experimental
features.
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

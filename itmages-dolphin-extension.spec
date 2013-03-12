%define name itmages-dolphin-extension
%define version 1.09.1
%define unmangled_version 1.09.1
%define release 1
%define _tempdir /tmp/tmpbuild
%define is_mandriva %(test -e /etc/mandriva-release && echo 1 || echo 0)
%define is_suse %(test -e /etc/SuSE-release && echo 1 || echo 0)

Summary: ITmages Dolphin extension.
Name: %{name}
Version: %{version}
Release: %{release}
Source0: %{name}-%{unmangled_version}.tar.gz
License: GNU General Public License (GPL)
Group: Productivity/Networking/Other
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-buildroot
Vendor: Khramtsov Vladimir <harestomper@gmail.com>
Url: https://code.launchpad.net/~itmages/itmages/itmages-dolphin-extension
Provides: itmages-dolphin
Requires: itmages-service
BuildRequires: qt
BuildRequires: make
BuildRequires: gcc
BuildRequires: gcc-c++

# FEDORA 15
%if 0%{?fedora_version} >= 14 || 0%{?centos_version} || 0%{?rhel_version} || 0%{?rhel}
BuildRequires: kdebase
BuildRequires: kdebase-libs
BuildRequires: qt-devel
BuildRequires: cmake
#BuildRequires: cmake-fedora
%endif

# FEDORA 14
#%if 0%{?fedora_version} == 14 || 0%{?centos_version} || 0%{?rhel_version} || 0%{?rhel}
#BuildRequires: pkgconfig
#%endif
 
## MANDRIVA 2010.1
%if 0%{?mandriva_version}
BuildRequires: kdebase4
BuildRequires: kdebase4-devel
BuildRequires: qt-creator
BuildRequires: qt4-common
%endif

%if 0%{?suse_version} > 0
BuildRequires: kdebase4
BuildRequires: qt-devel
BuildRequires: libqt4-devel
BuildRequires: kdebase4-workspace
%endif 


%description
This extension for the file manager Dolphin,
which allows you to quickly download your images
to free image hosting ITmages.ru, in "two clicks".
Your suggestions for improving the script and bug
reports can be left on the pages:
requests - https://blueprints.launchpad.net/itmages
bug tracker - https://bugs.launchpad.net/itmages

%prep
%setup -q

%build
  export PATH="$PATH:/usr/lib/qt4/bin/:/usr/lib64/qt4/bin"
  qmake -config releas
  make

%install
  make INSTALL_ROOT=${RPM_BUILD_ROOT} install

%clean
  rm -rf ${RPM_BUILD_ROOT}

%post   
    
%preun

%files
%defattr(-,root,root,-)
%{_bindir}/itmages-dolphin-extension
%{_datadir}/icons/hicolor/scalable/itmages.svg
%{_datadir}/itmages/itmages-dolphin-extension_ru.qm
%{_datadir}/kde4/services/ServiceMenus/itmages-dolphin-extension.desktop

%dir %{_bindir}
%dir %{_datadir}/itmages
%dir %{_datadir}
%dir %{_datadir}/icons
%dir %{_datadir}/icons/hicolor
%dir %{_datadir}/icons/hicolor/scalable
%dir %{_datadir}/kde4
%dir %{_datadir}/kde4/services
%dir %{_datadir}/kde4/services/ServiceMenus


%changelog
* Tue Oct 25 2011 - razrfalcon@gmail.com
- Huge update %{name}
* Thu Aug 08 2011 - harestomper@gmail.com
- Created new %{name}

Name:       org.tizen.setting-homescreen
#VCS_FROM:   profile/mobile/apps/native/ug-setting-homescreen-efl#4b7ca63ac5c18b22418b101a620672d62d13c598
#RS_Ver:    20160512_2 
Summary:    homescreen-setting
Version:    1.0.0
Release:    1
Group:      Applications/Core Applications
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz

BuildRequires:  pkgconfig(libtzplatform-config)
Requires(post):  /usr/bin/tpk-backend

%define internal_name org.tizen.setting-homescreen
%define preload_tpk_path %{TZ_SYS_RO_APP}/.preload-tpk 

%ifarch i386 i486 i586 i686 x86_64
%define target i386
%else
%ifarch arm armv7l aarch64
%define target arm
%else
%define target noarch
%endif
%endif

%description
profile/mobile/apps/native/ug-setting-homescreen-efl#4b7ca63ac5c18b22418b101a620672d62d13c598
This is a container package which have preload TPK files

%prep
%setup -q

%build

%install
rm -rf %{buildroot}
mkdir -p %{buildroot}/%{preload_tpk_path}
install %{internal_name}-%{version}-%{target}.tpk %{buildroot}/%{preload_tpk_path}/

%post

%files
%defattr(-,root,root,-)
%{preload_tpk_path}/*

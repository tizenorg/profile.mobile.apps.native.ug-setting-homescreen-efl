
Name:       ug-homescreen-setting-efl
Summary:    UI Gadget : homescreen-setting-efl
Version:    0.1.33
Release:    0
Group:      Applications/Core Applications
License:     Apache-2.0
Source0:    %{name}-%{version}.tar.gz


%if "%{?tizen_profile_name}"=="wearable"
ExcludeArch: %{arm} %ix86 x86_64
%endif

%if "%{?tizen_profile_name}"=="tv"
ExcludeArch: %{arm} %ix86 x86_64
%endif


BuildRequires: cmake
BuildRequires: edje-tools
BuildRequires: gettext-tools
BuildRequires: pkgconfig(elementary)
BuildRequires: pkgconfig(efl-extension)
BuildRequires: pkgconfig(appcore-efl)
BuildRequires: pkgconfig(edje)
BuildRequires: pkgconfig(evas)
BuildRequires: pkgconfig(ecore)
BuildRequires: pkgconfig(ui-gadget-1)
BuildRequires: pkgconfig(dlog)
BuildRequires: pkgconfig(vconf)
BuildRequires: pkgconfig(ail)
BuildRequires: pkgconfig(pkgmgr-info)
BuildRequires: pkgconfig(feedback)
BuildRequires: pkgconfig(capi-system-system-settings)
BuildRequires: pkgconfig(libtzplatform-config)

%description
Description: UI Gadget, homescreen-setting-efl

%prep
%setup -q

%build
%if 0%{?sec_build_binary_debug_enable}
export CFLAGS="$CFLAGS -DTIZEN_DEBUG_ENABLE"
export CXXFLAGS="$CXXFLAGS -DTIZEN_DEBUG_ENABLE"
export FFLAGS="$FFLAGS -DTIZEN_DEBUG_ENABLE"
%endif

cmake . -DCMAKE_INSTALL_PREFIX=%{TZ_SYS_RO_UG} \
		-DTZ_SYS_RO_PACKAGES=%{TZ_SYS_RO_PACKAGES}

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

%post
GOPTION="-g 6514"

mkdir -p /usr/ug/bin/
ln -sf /usr/bin/ug-client %{TZ_SYS_RO_UG}/bin/setting-homescreen-efl

%files
%manifest ug-homescreen-setting-efl.manifest


%{TZ_SYS_RO_UG}/lib/*
%{TZ_SYS_RO_UG}/res/*
%{TZ_SYS_RO_PACKAGES}/*

Name:       harbour-sms-share-plugin
Version:    1.0.7
Release:    1
License:    MIT
Summary:    Nemo transferengine plugin for sharing contact and other short data via SMS
Group:      System/Libraries
URL:        https://github.com/martonmiklos/harbour-sms-share-plugin
Source0:    %{name}-%{version}.tar.gz

BuildRequires: pkgconfig(Qt5Core)
BuildRequires: pkgconfig(Qt5Gui)
BuildRequires: pkgconfig(Qt5Svg)
BuildRequires: pkgconfig(Qt5Qml)
BuildRequires: pkgconfig(Qt5Quick)
BuildRequires: pkgconfig(nemotransferengine-qt5)
BuildRequires: pkgconfig(rpm)
BuildRequires: pkgconfig(popt)
BuildRequires: qt5-qttools
BuildRequires: qt5-qttools-linguist

%define smsshare_datadir  %{_datadir}/%{name}
%define smsshare_svg %{smsshare_datadir}/smsshare.svg

%description
%{summary}.

%prep
%setup -q -n %{name}-%{version}

%build
%qmake5
make %{?_smp_mflags}

%install
rm -rf %{buildroot}
%qmake5_install

%files
%defattr(-,root,root,-)
%{_libdir}/nemo-transferengine/plugins/libharboursmsshareplugin.so
%{smsshare_datadir}/translations/%{name}*.qm
%{smsshare_datadir}/*.qml
%{smsshare_svg}
 

Name: harbour-sms-share-plugin
Version: 1.0
Release: 1

# >> macros
# << macros

%{!?qtc_qmake:%define qtc_qmake %qmake}
%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}

Summary: Share contacts/url via SMS
Group: System/Libraries
License: LICENCE
URL: https://github.com/martonmiklos/harbour-sms-share-plugin
Source0: %{name}-%{version}.tar.gz
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires: pkgconfig(nemotransferengine-qt5)



Requires:  libnemotransferengine-qt5 >= 0.3.1
Requires:  declarative-transferengine-qt5 >= 0.0.44

%description
%{summary}.

%files
%defattr(-,root,root,-)
%{_libdir}/nemo-transferengine/plugins/*shareplugin.so
%{_datadir}/nemo-transferengine/plugins/SmsShare.qml



%prep
%setup -q -n %{name}-%{version}

%build

%qtc_qmake5

%qtc_make %{?_smp_mflags}

%install
rm -rf %{buildroot}
%qmake5_install

%changelog

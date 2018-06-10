Name: harbour-sms-share-plugin
Version: 1.0
Release: 1
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
#%{_datadir}/translations/nemotransferengine/*eng_en.qm

%package ts-devel
Summary:   Translation source for SMS share plugin
License:   TBD
Group:     System/Libraries

%description ts-devel
Translation source for SMS share plugin

%files ts-devel
%defattr(-,root,root,-)
%{_datadir}/translations/source/harbour_sms_share_plugin.ts

%package eng_en
Summary:   Translation to Engineering English for SMS share plugin
License:   MIT
Group:     System/Libraries

%description eng_en
Translation source for SMS share plugin

%files eng_en
%defattr(-,root,root,-)
%{_datadir}/translations/nemotransferengine/harbour_sms_share_plugin-eng_en.qm

# Hungarian translation package
%package hu_qm
Summary:   Translation to Hungarian for SMS share plugin
License:   MIT
Group:     System/Libraries

%description hu_qm
Translation source for SMS share plugin

%files hu_qm
%defattr(-,root,root,-)
%{_datadir}/translations/nemotransferengine/harbour_sms_share_plugin-hu.qm

%prep
%setup -q -n %{name}-%{version}

%build

%qmake5

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%qmake5_install

%changelog

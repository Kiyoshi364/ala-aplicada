NB. MY EDIT
NB. Note: run with 'jconsole -jprofile path/to/this/profile.ijs'

NB. J profile
NB. JFE sets BINPATH_z_ and ARGV_z_
NB. add your sentences in startup.ijs

systype=. 9!:12''
jpathsep_z_=: '/'&(('\' I.@:= ])})
BINPATH_z_=: jpathsep BINPATH_z_
omitversion=. 'Android' -: 0:`(UNAME"_)@.(0=4!:0<'UNAME_z_')''

NB. create SystemFolders
bin=. BINPATH
install=. (bin i: '/'){.bin
fhs=. (FHS"_)^:(0=4!:0<'FHS')(5=systype)*.0=#1!:0<BINPATH,'/../system/util/boot.ijs'
install=. (0&~:fhs){::install;install,'/share/j/',omitversion{::'9.03';'0'
install=. (INSTALLROOT"_)^:(0=4!:0<'INSTALLROOT') install

NB. MY EDIT
install=. install,'/share/j'

addons=. install,'/addons'
system=. install,'/system'
tools=. install,'/tools'
home=. >(systype-5){(2!:5'HOME');2!:5'USERPROFILE'
home=. >(0-:home){home;,'/'
isroot=. (0=#1!:0'/data') *. ('root'-:2!:5'USER') +. (<home) e. '/var/root';'/root';'';,'/'
userx=. omitversion{::'/j903-user';'/j-user'
user=. home,userx
user=. >isroot{user;install,'/user'
home=. >isroot{home;install
break=. user,'/break'
config=. user,'/config'
snap=. user,'/snap'
temp=. user,'/temp'
temp=. >isroot{temp;(*#1!:0'/tmp'){::'/tmp';~(0-:2!:5'TMPDIR'){::(2!:5'TMPDIR');temp
ids=. ;:'addons bin break config home install snap system tools temp user'

0!:0 :: ] <(({.~ i:&'/') jpathsep >{.4!:3''),'/profilex.ijs' NB. override
0!:0 :: ] ^:(0=#1!:0 (({.~ i:&'/') jpathsep >{.4!:3''),'/startup_android.ijs') <home,>(systype-5){'/.jprofile.ijs';'/_jprofile.ijs' NB. override per user except for standalone script

SystemFolders_j_=: ids,.jpathsep@".&.>ids

md=. 3 : 0 NB. recursive makedir
a=. jpathsep y,'/'
if. ('root'-:2!:5'USER') +. ('//'-:2{.a)+.('/root/'-:6{.a)+.('/var/root/'-:10{.a)+.('/usr/'-:5{.a)+.('/tmp'-:a) do. return. end. NB. installed under / /root /usr
if. -.#1!:0 }:a do.
  for_n. I. a='/' do. 1!:5 :: [ <n{.a end.
end.
)

NB. try to ensure user folders exist
md user,'/projects'
md break
md config
md snap
md temp

NB. boot up J and load startup.ijs if it exists
0!:0 <jpathsep (4!:55 (;:'systype fhs isroot userx ids md omitversion'), ids)]system,'/util/boot.ijs'

NB. MY EDIT
SystemFolders_j_=: ('addons';'/home/shikiyo/selfinstalled/j903/addons'),SystemFolders_j_


/*! @file ACEngine/main_ACEngine.cpp
 *  This file contains the testing code for the engine or..larger project code, like a game.
 *  
 *  @todo Add the global variable list from my older engine
 *  @todo Add AELogParser to parse the engine's (AELogger) log files
 *  @todo Add engine's file format, AEFile
 *  @todo Transfer and adapt code from my previous engine attempt
 *  @todo Add project documentation (explanation, and examples for example) to the readme.md files
 *  
 *  Should not cause everything to break :)
 */

#include "AEBigint.hpp"
#include "jeaiii_to_text.h"
using namespace std;


std::string_view getStr();

void outputBint(const AEBigint& bint) {
	cout << "--------------------------------------------" << NLC;
	cout << "The size of the bigint (digits): " << bint.size()<<NLC;
	cout << "The amount of number sectors: " << bint.sectorAmount() << NLC;
	cout << "The memory usage of the bigint (bytes): " << bint.getMemoryUsage() << " (sizeof(AEBigint) = " <<sizeof(AEBigint)<<")" << NLC;
	cout << "The bigint is negative: " << ace::utils::boolToString(bint.isNegative()) << NLC;
	cout << "The value of the bigint: " << bint << NLC;
	cout << "--------------------------------------------" << NLC;

}

constexpr long double runningAvg(const int amt, const long double val, const long double newval) noexcept{
	return (val * amt + newval) / (amt + 1);
}


void benchmark() {

	AEBigint num;
// 	num.setDigit(ullint(1024)*1024*1024*4, 1);
// 	for (ullint i = 0; i < num.sectorAmount(); i++) {
// 		num.setSector(i, std::rand() * powerOf10Table[9] + std::rand());
// 	}

	//num = getStr();

	//cout << (num.size()) << " " << (((num.sectorAmount() - 1) * _AEBI_MAX_SECTOR_STORE_DIGITS + ace::math::lengthOfInt(num.getLastSector()))) << NLC;

	ullint repeat = 0;
	constexpr int iter = 1024*128;
	std::string str;
	timePoint<SteadyClock> tp1;
	long double tm1 = 0, tm2 = 0, avg = 0;

	char* cstr = new char[num.size() + 1];

	for (;;) {

		cout << "Starting timing string processing (optimised)..." << NLC;
		tp1 = getSteadyTime();
		//cout<<a<<NLC;
		//outputBint(a);
		for (int i = 0; i < iter; i++) {
			//num.copyFromString2(getStr());
		}
		cout << "time used processing (optimised): " << (tm2 = timeBetween(tp1, getSteadyTime(), long double) * 1000.0L) << NLC;

		str.clear();
		str.shrink_to_fit();

		cout << "Starting timing string processing (unoptimised)..." << NLC;
		//cin.get();
		tp1 = getSteadyTime();
		for (int i = 0; i < iter; i++) {
			//num.copyFromString(getStr());
		}
		//cin.get();
		cout << "time used processing (unoptimised): " << (tm1 = timeBetween(tp1, getSteadyTime(), long double) * 1000.0L) << NLC;

				
		str.clear();
		str.shrink_to_fit();



		cout << "Improvement: " << ((tm1 - tm2) / tm2 * 100) << "%; average: " << (avg = runningAvg(repeat++, avg, ((tm1 - tm2) / tm2 * 100))) << NLC << NLC;

	}

	delete[] cstr;
}


int main() {
	
	cout << ace::utils::isInRange(0, 9, 5) << " " << ace::utils::isInRange(9, 0, 5) << " " << ace::utils::isInRange(9, 0, 15) << NLC;

	std::srand(1);

	AEBigint a = UINT_MAX;
	outputBint(a);
	outputBint(0);
	outputBint(LLINT_MIN);
	outputBint(ULLINT_MAX);

	a.copyFromString2(getStr());

	cout << (a.toString() == getStr()) << NLC;

	outputBint(a);

// 	a = getStr();
// 
// 	cout << (getStr() == a.toString()) << NLC;
// 
//  	cout << getStr() << NLC << NLC << a.toString() << NLC;
// 
// 	char str[10];
// 	std::memset(str, '0', 10);
// 
// 	jeaiii::to_text_from_integer(str, 10);
// 
// 	cout << str << NLC;

	benchmark();

	cin.get();
	return 0;
}


std::string_view getStr() {
/*	return "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890";*/


	return "5858272927315393276614521125294896712770267462687723420955797852784556430528912391279946167776327016293185624728977103426140611875568698981271515128373451534539775127154557420782109635016923751402739841006917369665759447135449694477311186789599111644004607185739453021485663101057608872966397347142457357707480540010137982950751904029288060815600205306870380498318901873960470609185589249727434471280210379313078804192358234099500126723281760854481953110227117066507834051890346588505722866279110206739406641802987780532510501737570602280951512948231211428744477838946347519767904292114604587960184261888715908927905150939336531163415685107954889486583490216018537396556744774817940933132720651287187980513012264891344503050259212250420014744150259838136791371412102064521162596438182936859153091569640395384258619136607336502685008023405627098181321970508917254890757351593538141218243472949936212129806591366228647295812632296290100621030383795278105524970395177017650056604211583622218270252225973130872376670277470007617013468263156831591060561818249457951666913063351843516316044772546416289857752656247056266694560231926275907545563937765039574395113496424031034480156629067761146791465600117954832384597705459377374640526139377737890060335351498919338564566619224096828876551143454148568101930017450009565890026016566060166055095150337835714547449846959051173511732757270333658085091360398208973832915133474045360005970440495732853192087368175671955984407088426993372794904343190402296943175265296502268424169673503836325814914203017359371109708889807463102064814671082259624117650416360610521220618456342678498662048867494541643626386148194985963738139021315307579363960544019172344305185815695587481212771328860827729422909322655244510403209588143361474829603054111239225934188407061850899452824124399194226965900015749354946527252631238289128977675594112411956734937320724910852066773277672357937741514055775935003022253516162274241467961744877086803779494619044018790908456794337551724566722280955039166535578684358639892953682566550062"
		"6118748536231262698929863872843280112003955735554258366040545122568956968145759541738810381789312986859841381293669056552005783112937112389910046359018157578103101982444410987326473649796827785381896093869873331939924702013313702468067254918549365232665588260070114031026921156381151836565759325526464228089123779123861048476072895507103661347096895865742505827017135312513587866531527060837039339700741131825927936224412559312253372609264203762454202758593874751537769658070787087142777678761465993462918413589792019507289190033952995820175153909384797251877063352291946153400578006438218801221623400947019845185375048046566304636719730772768806216153133923679500353499684419569067951537021772502237213604784782845191546253592925567673551294525141543152518223950418087541431486053899676722787088172095013141168586395659858665082903255068381940160489190391481986192683779454704955789895525189136937703516594650712671699177316788138042963206316819893995897075462968903476632921755206003080345106700510473522425672704009451779423353755647329163367208834062934621080189859309919456613343322308928568576996503521946698155935295706033812446271303969157309292263977326469597029902264010549828890023527108709075660401532051127624452437608861633677461964936322662752866755994172378460005628742608883012653562782908638448943340885571558557530952578893784502192872305775408395688900277022420172939941204073334666577686769139231030335069839097430231793952721109682489990296845284937149185432582061982502838778661720007494995436098689143053727659918506476271287828847310303595953368060174519688600775987911730397056054077699442270821806696352014941105328445584949844675755308965067487159203454940117180356116391028752670132610010473920261691850345180673350810001602781473472147387068615473118427945496211789691190257214401757139783688530687537902355793844733646415263820239477686634225562330429757679808469644803373801910851413502056503130740388417596919347619783761381937190640813054063292255769066354252057955494540466304196471261217359067377509824639812322"
		"1073559368853481481668763287773415111494217674377182334018715177637790607089563497540987231205480839004395785263566591441438620250099931937226958397748456539312834209454020103667258254771641565196770722966397274658968026791582616966362681434915269749800430254317674924956194506914855948227637000064815716928505454507975665694978288850121325220157781013792190930851681660403121351566362921737819726608967058310741707066196690361376737935612315668147561217375586481575180061120847419766068431558107448021338290744639149959643340934639213112284479132923113567852526115352589590575525994197267964587554739164472188685761297159852778603860537132574366693989139025198678874846977561375349970097119423995727961152394078873456797865024259009289976722985709494543830741464968441519691727782071144228140788340106850079258145913728417587933848059185351223004655049851747605925136820110597699482322086457969933824695588860924529451096701732642660271756587600241705250217931199307065476114068522698790609261292532299719927513860170879700151375626489275308752458763285195051755498576724158499119497884484619118135694504674164410175729464093562489036206872309949038080999715453443313149746282487347326536905181304929133981008346945549121330556206867408496663749276837094058047287874300361386948157097619606980232163191243096379796905331940441411830249573625345439858382503307451327152191586963368922919653770123581964727038699113410415222087294927084126142199312590480399802022623492911839367393858606747076906576164421574532604915608555480694597125701746899287827087786946834008254236453044209902999983261320148101160387860347212046798386508526885839891897450605191111833955101847254113471545816141127359688799891418907066948719457751869527462267651084464603405806003052640078143810575953045708830795677495799159810840456467714532816673549855427098850327386253823867148955861191367103641208690948075914623995851560062099303260307257620604370123177498833220661827379152357310064212237145759701971976084859723318623881535284813375579355280736215606095955420402868"
		"9138367242375206819872025347255838867857606517117174596163469404876804844851095287514618991878188853864807280125436669033009225365370888569165912904219408767016253753818911996221504911467647294504567488459501256147017417384909178375631711621542034529126607120987903392273270838607434564385239689825287431960039734651218300620877672029639029483570451414876418183077645056011358860219443481961071882783823723955278171966953619418115450174436128082946063949782134611386612692661633670410388351110081692665968365951740325790770860236722200421252039700333710915549462868932453770789873771099299600954969675526207868126314002749212817630655020981858822046266283682590192251162423231154136642793077004889910868875171444694057627844877446915474141716569710328131257886648561138897897825009374338189099158627761270645607626798281899536025964549133347227226521204222977482157120554922504676168130491098280407597451684432532089060234692586748369530097813709412944691787121194825625429789417519565725160253441953878935020585229788141930078525984869663884114088220005865231550415811330549817533601206717043767514627019427287638023112436531644310523437067255943784057526523755672698580115933144109867058943027591578725830837011244414252542918033366791617032330930449515842008628618002102221208047802406467093344217027121720841192032240574550801275430720669500781821682085788851151593717877371052095680056277291140617782919620570748520649988363918512026583986115956697579017075876549275912218312346300550060414337683257354791449539555359326799709439746785525060610899266955242829709361927208618399160850809296323703479389862930306503116453487839862062910179898674362099326461733880253389273575481678840625534641673283729203470402685217780708674793489900266389159064872367421105732412038341767222672524486728712381321654924710716657503621785459287056903744197814035151695802102688486116508553824696861073449857036737475764662802010503401845445271149085904854000233696718810799818287945283611643949343155335747988682133275659248515476300453660746221189742293779970"
		"0759200734390917612860532152017817073524478255657855524649346138419001466908504227302877413760483702886645374739029901875380109596641907876706885500469415110372282164248486389365000178138980394817316620734789324535664968210391900599565299157199318342259426427497699142882555600149615903930976556747868431088859426499134007621671845488692268381630165189777246977309983491571451886083923100419561167831306935446827553213146578964348553822458852106189504576379517158758762377024249264682644050725031402654748067629482899108319571105286819777281189500203970760264656811218221958750193653615586748407881425689594541899566445956118902318921671452927630719527706890166681930305909611498908032052750070545889848132629543290186465244737353966949312295521310029928394350757812254862181680862921842661128599396408197545340602663450414611346597002825504981946231879258581943253254330375017148873122451685019457379430193197176534563927796744055986557596825469036085680972684464130759036085454858003948849189567641745554789234136003225427219220965184434937318573015345637753579951534074103514553844262296449891901390579203894884252016855867591043927896032162389524697805965637256060355700042315899594587796847745636563229311476040335324625577471343319838516457075250123036559739050047106536670813074449104679352097999780902159102963425829369131202343264613103462238550385996625863835425937194250023942305054446087544396236095031268977514283812799514282890290461382018681027008470289636495142762564470700462625312503097901972573585470558395414680040832833533119013610836055917934663971086048692396046681241968382577756262400623905868730963671250283522755816759136322604855350138699752916178304273783825655982034691656273929333730705950336339729214243070303945599579922476669810884016092284606761491774910450934380834824932651462810446357199270894039286166631770530217710685579323785572301985400407390167351639997813013954061000010746946308663798741456620482483917506429152530352670424959762175403322816028212882821597624951627371356556999641228385264542165442389"
		"6819014650052421671226262916970941013552130106326535672627110538841439087668492765162377424293559217379814470507074529853813790232150176966670230495907527846339866957399983228517362143511612712185540986320550404090312798413412544749983299073680543093936518283811187686059930619014256767841533524910781649434925622766306193039376606725144900005859547892862808558577613415532573449212303483002230833885373996855008713660842190809298858672331467180695103813525468398239236083226025790701625101656775347567068936945633002714823233576456056521790402704944261065938691914145152920726680166189742693234181000997968332430172633689304412129290957465748523184078711227297725300164364708794270175311551376040007098935729693469497337731005335294200852521285095921253882388697706076958117404678748620082166767754458523261799573362315323120065388881856525529924035900183781165303614168759383810499996855114639000996609293740304537545251433914470598253213388927078228183021769881724366007335940807536303546507219545876808536488280957759008721426977351152560787402059002732655785194241428795839031436156550916479861735888977996933738908228138650111137677669725733234525343452145497838665520435599325231832762875632996780622106674318650633913191492978530257263573573210841732189865945157340890401035908955018018104380317980676530145470510319942904258527996880112003747262179676115940987149456315808156377986671792746231996503903840758549514603122340002003530062562787003190907015892959615422587568847659776424024179712406212511165243355713039770061354316654715727582182469684079443311774323332004960010027624105300981551021800605897726263457953350412427845820628848871284287572431180670184544630292840075056736965092673275096693021514397685043899704600246083797788057377762144210732324927751303400371408368861095771693882661422440540121191616811336085849021400279469263560298372646411816049045292647422258424827550914358299198409181433937820333683088816645583383945446395126349386218958113607682959606297415583749542996871621445597556136049799150907681335469959863"
		"8030515741118589784042523251963638030545948579884673667954894341120621550758450148974811715587517656847213195895500394354738556143240720289247274762578891841608024498574815178228364211830087618992093326935986722002089457821372505864477187600237506652449354249264465915738586988146874112702408528829719476390653542331262608825080713364653290445144381945443996063431250194951808574640550019218875208955716888820686943205598400195298548663387824555675638546327217142186051422498953364416337499944139745247211587991877167591502828596589696212067049615825832669993975273899093108909655240182125361974589695899837862745267302139958302597202834356837193226531679020866315885844117941328335985550505044204788559413347343388652552397574184038715347227014390494402946409511448056628106139393928087056409514855736195070866433210620957375709991657936571607163247253005369543842579520129570369138996886237396077625942172146733602894274200810561999541270791426490707870896636122529735425880524320720519596391263135077321430191617748198128086038172438320478254620444213379156396054632796540943858711040564743717732506451710599475780878858024670833967561985410422033981497874204560903728132025818737520536330142670164402813606094402126426982098224113729219138731262883373631463009139339788549181181432832885845863990166889122998672564230091398540608256332869526527289809934743771627302101402904751473848506300667657391003603699002683327025892609999157215184495052850500629214010957496077463064454807770650348482303007543965637338779959802255528985001675199491886683700834023341769303473475136668317865306771817060830491761813731936772720301305356181090626264640370377010752412605562967413037699144720476410153471181304872395221742907895486461763945519462719733509739746882961115303378599811639780750732153907418331743215835988399052659462188056820515260193582254191621018654392839439322742368291202000355172810335561746615314782959369598230955576640137878078547956072022322481002029890741149541237973748383661865250228344816622323785746011116845584985624858991699"
		"0358591782293386008019343464541195492111526316177638533800265357657047705269297895524191855229361016173883037569033479857212312464863939044533821166177054882726423404064676651665071868847622807222437022423844496832706268922325797269887572200996950135677365865575522981094826424963851488527548054483216298241634663479752202384857347288262047506601016027953283868585654443392605622143045445479973458121165609024792538554227995208254100372277215675638869634613305275213416030287755368971366370599815520907016074954835461028789156345279685114393250559948519873959214830860588012958433260052109330604801615607762924880422996917989987976609995105078494359054245038158794822696454424413986830442943233057611762692976812727170691222064817843122663937943176837688798092550893463180020447709667524666548745714996987623055841282476737002219982531969597229045149614255847813482971171664292384781770410992045164483801931637239570761671519097953754741843470713359741474838052556252701901172536165577533548092015417109083057968709388748502959720637682327112514022305011863341068812942655060927376850756116859949195217038156895053261857635234675322828452518416447838159145445278830798711509317887143941210195682379883089529078913336683689395451427885851597290400310031065153243324045274201099855750428761469981839870943416757096479221068191103971027816147546813425841666440995137793158619339408798259946719360417065483626638463961076246711305809864282501421925331682688892445342252307924752192597762848985914935460527370743074780754147707378679555992851589264817339129447912250967026582734610840223453659360779306454442190562920055813824610553736240153188116584203412530736392033875093943080267479948927105346083127587441279020426632146647893195965299182823803067828206012859646025128723920807553713668010465999599205886597229267995695493572897488038689643520055493752094281011238873821798874915498530302346411387819576155847553345024336793639998214466285458807911044344002765462198749916900213552480648465769876832023908400462092874206082697702353827522388345278"
		"0250833564684432370850329139863595236471642878817288855808237532079610094253429748720803126526968695894222567821980516651351790712306968286520374415682440779260139133872124421706576159820836545301558802315359352994211662327428507842127322967080806524649254727172688058802553447229457985093157529803145969166756429861282040720851635620699437975261419127173403320046620093354368698124401858489620717411217679157124627864488821391380811767457851248638650487568556376086483652260843386661875836934858716133056518418687731303256183174274649681100079863344346156165804510204184898333024898686479922912266836278910182493122155439972649587433032657838173657829330505231433583780146014621590599450578821527780545404409739245979434212877583446810477994134111308361712360214507432286387157390849844717081906690729246693208523762250544338430802209583919733137890405640992491528973084517454137300759838246405810633282861491019295064598388665378145361198064772157079741309803916932436043709248371315406658841023785336409538193667444720101796478700688845854331218218190369257407445174564992102732627826331161173260183033912935037909062892060638646005434629880898579587026389498653372366807968051999124937349228553511956729853373487364827586464903575852313621188525452706139042586917158905910032898431953992586416643820770132910076173997476465616178892903526289952724819553428874237684025925605751817747230553598694090265681037372754464696115968990300717791476531755522833585530215698410430532320989500072302133340627414351898093748053933473969518019579765564075593020228758378866424167149707235335769047807358829904096555100234017931215753644208357823666970601936840782741305373588109441700864651695245201725024240902946221625201558727768863806819987283920822307266183483537587430973032017986179922842743179807982314690479532482736675902298345806493373373155961705843523822738652913515118485952879379223980555287123909873300285570449590870379386951771211334944459739856155803090082999780108529538987103445753520443428855674596261972422337383679429982299060612859"
		"8561282302265834395200849884115279464778833584219355729171698954600871099213627095005094221411542021062043903708751247872235341376506319410138394330440595361242931249330905483577981893366185151798054962667821483518099219324292031484636311615139565477405301252058040879568755146739820065959001464122303151391040679033438369013657850944913775661762937382773004733425391356956717648078710318555916155761994523980526741746444361861741539017257036668099517122183978026886606023780935853320269800253254154663029524219685330937700938026546645324128363249707700846550591124510438055306046358272214820455330415106048295336212697608265961479580083795165540866777278619254264385404185284289200031292499007895494580919246786661286736039859970234647911747632254856874345459826621623966302437004988914690437844050037184441399491245297321557673338443643725222671741225819277098424166621605191621975370542600634918515761511373815972004498356076457776620308684239779806008243468305095471689589134441164894603958241702599357712143208178435501101889463392166170290669705085567701941806579236897480711351692166440542215230095451868478273582673029401589018907521565388806714852980548193090069592183528915225237183061211063031609909006528344742815174218064471345026000457004439885637268181929028994209458749265253857558549710002189336708652402529604199300199444700414045721761439370769384368025845279698869784325250569064753376222815974705584058962254088711488277559523264362043902826258083561482270863636428520486008409177284897580261558925787344108475974147590103888560223207136685138584179758691492236695024216878818735762713304460235702851835533926033786604845979466046038166639810367094902087352742257528738759252861350759762546978214341735862284531235005849614771456946843658326038435919758498033452520573778870818876800274248784923144295750492413879626978058138203698395033836564641538944444176909691285472079333349051675128000123322135475492498027708596046948054304848439345986460387613355701426938382874336670378999934774445502458215501712601481160088350069624"
		"3943338984391097012698627289538019616613402399209881562747652139957103744898024475587856196920597457252593199392893875195999395210757788747491587086433956169758079512724612950345236656418025956693979251073508389223489335910518823629811567532654080035495879031575515060006058635123688819079875129453058127859909981574047919680034424623464430523779429980087677937166601394582246649750156878617141372138507837742197575247927813213984181019501900608435675893312181483673639345707344436744870387029774423700107821693303452807728581272114915438245263352273789271382979204258476275976008700318891791923134745934392481483033165211937769557741820798405262408857773074380057847918973836949566421243269789090936890271256410881377936802553025006081279329775997161228381791740537333830010740905182491338365454256303023685191444018133460077867571212401574295983933659448312558838870057174857787457607682108510145777299633711586190120876166974453361286179927467563586230757259390146282854921882954809524279484582818078472723848334431061461580397600691158907936622244150058088370376998234041862200661412612911905920654540102769614415701868392337104025782087350950444091493266392177677431329475513129985257818542049000250160265231899552615243475512189553498514275895413081312236339213782196644870701587256756930541279838855553511339559051598716531388660031623821892924682137729168999138971650897929815893985607082736839493666155039602627790668431620073686434020283198880449111251375797067735644848603581011316953899663739408261257781846836363382751619520571019687943985574260204400232677747952035900760488939994155325394028310092574756203870738488518466989766638452695601834950218951193431166365350166738223216265437862193101513599361554555257780155725780410296779431735265881627631784966052955055137838085986246782098638379156077795760815008403119012085315620389490919339460217494761584735463409503917502526783861283081818015916727015328908100559116082408056195363226454400893711578040935380627177333063597662202057823167001395928417314513310585039024786906749036"
		"6373848806090227220292153805652850468520692548271936017434365459694258622838564221937468407124716819232117126907988469737194235558762910945065627246410484411832188585706278164844801985443674748903422353600370984730093023979400808281403028685281053723365569526532255598129752789457334469426457794734023897740096160358214849152061447618340412338214574600073155861540529604701386732566728706953439064110451550023865618645458429050074543844451733000452123005519301539826060180402690786721148134676185670773746388344468782183657333684862769491089397512655502889753164326309868873931824220331631595474035951385307229822517892352443616121717109636268956692157036054392946587870412622615173791584999386367416715774669867963144153188803495707340093836717268961221318989951425668187816965629899849231794830723837521654531598789940855929865344697778213131736131347112313107292149152909393185041911522607346507955438265843012351042560626690046984476276759300393672084403469711705908268908448746546179930181499912065683342442818660056178112790829571809994065668223985358889243340425927103661449941393508334752518534364813476078663364499931089896233672164672867820099089446045758276400574475901510780374029127052093883371475888701114972672706278434021498763342163908677465959354177797455198461886303161762719942056122258757181243034143437020641510610591282792784279505124953425226112159380490319728560237657978422425243337430995319487191808051334540699212972045566943762841703376413207420106291915824770429236398082077314150890664203980663899205169166583228432119478802246355628866039326527895094711047621716179548743726162052149390100189015331529536284104916345880003761125670237062877418900655142183874025353209413401444800560340254686892035264594042523325802910673200511746088120790038558906494227639749406739056275106566010415664007242434283100036572247874287590568243956332863573798371776774923182824057131529730188312978409034108697129787751325958623921566540708572447070452626683201022452408445525607428585776518826802231952836383749330207041207242319877"
		"4329306095305292721548921553336453878254955233956481967437618708872908705580873511574405151175778268917110816853284939523923166057786890176796885069610742062532287476496445927003222635315565076418099974245951425533754244630377239476572352036080785986427033445463392751486237517630769994321256549228871095329730483683867985423890296901561652329296144265505808728631290812657235416436406559047503171021148321212332792859650454729726305732491203646805543490122954452583057795480213922231640628207776003048083155683151270268757363193779073974586845651293686922293004632592715527822543775885727231186846803779889924342837781457110868469346669079261704630155392010132373408898287506002601490815133991238188491281816005398739239715657528924610414238651567034477522888194592473045240274493437330263971947778167559775931967966299817374177276663592113592130295607374064094550352152162830161615449708260504713397846692721864407708704501167425967947334890646485402200623275012423796783358114538375312207829883119307692462708154483691252570565553813724455030850644652124661643400213004443732334375017879318241189313619533009943389842290788938342209634315807921129398785657537082809644179959090314685491425014622181821555767117052436039196538045384991715023079115177028171546403184643028817160670473137543905624094188905536823148465935887630532273760455006044361199264350218109056065873652804919865064988009669874504005147027579640862450130039876441281925893027348218992568948019624247990236001964985772232717741783506450441076660115018690513553255301399468906161336136713895765187969638981396460172757522967515852156839347471165622259360553052751869758815822724502308841388350436854268865495489507424345506071527338111154278955157512660159022694189044753027071346066937244917912493698678983825053810525946454237708279228374910852188673351671738451883664649971570576281572164450668129789212759697747010044212488536402788825837081252497035857901817696496069076452207869790856552146615895439650828693411176982509851350952792053284789192388655909165647976604271256"
		"8985403021813803593656758644683104194974377208484332552684875720617868726698119329372503265126193446094271283720226318733976595032995312810150138670786746567589803427132326407402540387171101849719159553630123984691885225113685383149307470666579755098030360765251312489069541154582054553479073968981111475566767615056323699582992914150113459554377182336203038403210860315095270343267961514327587477484604908119154372196062079544199807343105983895933665935135063721393974350071538794131136057350981351902536192321708884041107121654010900469718535413576681001346113133116667930651338956062736802562014733499977500081399929324059504100213421530244209428865278598722694375970610271782365977813518268250833290813822091747385853265940474333704282902555389253746468852022225720427963459464184621040691144659226254903238638201004609133089369509061907133281672761310122220277396853752788042923718694162451926375521494533668840068749132811648073704758564234381463614211736987562691618394935805971869175943471118661618979393401770691758034772603892637746862339540674447569014560657271294333478308681946348872692713104468591739591899675562775286462592351569609802421412429338457125662222541812574336215375136998514986028279341893801611428655545720690086151549314775211204781614454439032331343429699680435660345976968727741088556345123958342759384196074545647086571158819245301925081425788928146169533479746281063350587184682188497460719524979622179338010773971498777225987438698596658253926327451520835904615964613165586154248472845305115077528655879107511478852819308236380437461565039642607328834944035035346865343064326848894670023555092774030415197995393622774215200447830023754566604975747662045272265577422115667075712301946953885461876426812636196833573281949976384359300811982727800976190725507323577687984140275968108598376062821110502593688852432267948051091807430683666324782368989371220312177342490496298102676988978272696498874544426548582039051469960147070327150606459206727519482708503461560421071501431943198990162680046646804738407690289054960"
		"2236401694032125943328981934235818594661837108104098062836895059418435544935067711218678764032867561621255501484459578675622947503297963948707351849242464618750846556019732315005858279944392781230507032524023730715742127118215585589619568269223777128354856202062398348489917421483098437102030490551781805241403046292535058502146163838642780318524764288285284617031422142540541127525596796481280844244520936679915306240300420852038838904045877600474662951039708562706716418176989724604758077687142729513142014647011713230978843137577892036738995153362275770121526496460997134169551720978162851644291056328520229115722292860604716369420770406720473436809971491833117582867123890029091836003779700864157720548127434547306898994930101367230737501503648538668477585781128906689740110514503715924883812598415497115720895709150104022117257040320330047022835693060274275619657941848725001279195007846235169597513610670147199727681724680543368181381620768936431902778740667423833653239534216004012970083016898240399852212519412397017687869852709486845962419377701364078201189960387994963917208536162195650449269669288464002300196062204246226786259301942528530158167393607719740092211946789757756813320501505578789893314400665143348740307476567812683132255369447908243706330631337836641764818352208414673749049153983679627440917947374480426086265896636229072144418048333781128363956077615861348627994312867917387583493740398259651282576817811982082166919564527281095028537573826598081300350834976711847754525843056202631607023907790565971599537043827653819571646344369706008430372053137859706187820114173591689768620279494404246870507720935771520977777708037074310590919603908257042721597084949104416745864082953419798056465234818735547029643566390342611468531846062601439588601106117026818574384501486030701115887754584724629676950131060181183765669965217901446046442895320024414311907596247176152561322700955164537360573838171309667274470722456318254374628264998864806205255297915732382233242273936306433441066414153049714900418354805881276809996028254382"
		"2360393843391350089536775197410085292274308849115308560776917695076482284795167850905426318459820879386813732491798042717440613223440219213600167828560348732331288361968373154820545002160653269768103157300769043372127979001116188021656913944302728909029884557703210644776171435229107468166498070381338076269761855894592922297196762653434991766006829103231382130400000876688798033328392585674049310159301001490373322949964068984348142534155300895849515784876738558710428134156734430891050503415788496514491155339847141019099118803729854814932961563112187305158527854926104928576146004430033113541426048780582751341707464797573808076812690862206698603956437405025369256469061884878914786786889658165328783813854468674122435659763035943521559524153409101682502170356570600158220107453563747592272902358401977112201666094124067407484723241684899053595316963337055831237316289649104379245821354775239653285761012040901711317815111412827749845247285488067587197575265675863910059829252435283307597233504200080265311817214188244840935260072709220660099973102543611022364022942576768703618203746461035264586136597469521924471523458319879330448566279412199168614651680289140926057512730201620461427012845663436176887340918883247330238755510186156588872403757036235529849998585562555957699951338310279632011610603745594650093765268843316581170840226669767643149000719853422895105687381896110177817750983279987226647288771334731305765737526226019722005222842830512793056465723453368321473219065720653505037333623246070790706792765283707241272939903466045987041613111284087399131505887715023584554011479391311926321923959092572188615937909930461453880715670324946490038985045346513995267288358925745178485182953941629387814673194604559893053967340178068924926056298618886342125512427781212503602241462260604689670154783115291070120790517702442736608523834962743063705819137719697871365872829217001338571842685447607229404660806765670731021722685511254146050201541637716778465132136276689092359799578211846760069335386844333820793345136487406906720726515253155"
		"397579311771027";
}


#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H
float SENSOR_LOOKUP_TABLE[256] = {
	-6.775026429251402e+38,
	-6.989667978108616e+38,
	-7.21023623092436e+38,
	-7.436870908533549e+38,
	-7.669714465968045e+38,
	-7.908912135094686e+38,
	-8.154611967750036e+38,
	-8.40696487937575e+38,
	-8.666124693158389e+38,
	-8.932248184677563e+38,
	-9.205495127066276e+38,
	-9.486028336687417e+38,
	-9.774013719330259e+38,
	-1.006962031693095e+39,
	-1.0373020354820888e+39,
	-1.0684389289506999e+39,
	-1.1003905856987867e+39,
	-1.1331752121609682e+39,
	-1.1668113525466078e+39,
	-1.2013178938345813e+39,
	-1.2367140708232368e+39,
	-1.2730194712359513e+39,
	-1.3102540408826874e+39,
	-1.3484380888779627e+39,
	-1.3875922929156393e+39,
	-1.4277377046009427e+39,
	-1.4688957548401234e+39,
	-1.5110882592881806e+39,
	-1.5543374238550536e+39,
	-1.598665850270705e+39,
	-1.644096541709513e+39,
	-1.6906529084743903e+39,
	-1.7383587737410533e+39,
	-1.787238379362864e+39,
	-1.8373163917366652e+39,
	-1.8886179077300458e+39,
	-1.9411684606704468e+39,
	-1.9949940263965515e+39,
	-2.0501210293723807e+39,
	-2.1065763488645355e+39,
	-2.1643873251830033e+39,
	-2.2235817659859806e+39,
	-2.2841879526491384e+39,
	-2.3462346466997664e+39,
	-2.4097510963162425e+39,
	-2.4747670428932646e+39,
	-2.5413127276732802e+39,
	-2.6094188984445747e+39,
	-2.67911681630644e+39,
	-2.750438262501892e+39,
	-2.8234155453183725e+39,
	-2.8980815070568823e+39,
	-2.974469531070015e+39,
	-3.052613548869317e+39,
	-3.132548047302451e+39,
	-3.214308075800602e+39,
	-3.297929253696601e+39,
	-3.3834477776142005e+39,
	-3.4709004289289846e+39,
	-3.560324581301357e+39,
	-3.6517582082820936e+39,
	-3.745239890990882e+39,
	-3.8408088258683714e+39,
	-3.9385048325021396e+39,
	-4.0383683615270984e+39,
	-4.140440502600774e+39,
	-4.24476299245394e+39,
	-4.351378223017093e+39,
	-4.4603292496232213e+39,
	-4.571659799287365e+39,
	-4.685414279063422e+39,
	-4.801637784478715e+39,
	-4.920376108046742e+39,
	-5.0416757478586663e+39,
	-5.165583916253963e+39,
	-5.292148548570745e+39,
	-5.421418311976243e+39,
	-5.553442614377935e+39,
	-5.6882716134158e+39,
	-5.8259562255362e+39,
	-5.966548135147884e+39,
	-6.110099803860596e+39,
	-6.256664479806789e+39,
	-6.406296207046948e+39,
	-6.559049835059005e+39,
	-6.714981028312376e+39,
	-6.874146275927061e+39,
	-7.036602901418398e+39,
	-7.202409072527892e+39,
	-7.371623811140674e+39,
	-7.544307003290078e+39,
	-7.720519409249847e+39,
	-7.900322673714504e+39,
	-8.083779336068328e+39,
	-8.270952840743552e+39,
	-8.461907547668203e+39,
	-8.656708742804163e+39,
	-8.855422648775928e+39,
	-9.058116435590608e+39,
	-9.2648582314497e+39,
	-9.475717133653098e+39,
	-9.690763219595937e+39,
	-9.910067557858761e+39,
	-1.0133702219391522e+40,
	-1.0361740288791972e+40,
	-1.059425587567898e+40,
	-1.0831324126161251e+40,
	-1.107302123440207e+40,
	-1.1319424454280482e+40,
	-1.1570612111149609e+40,
	-1.1826663613692452e+40,
	-1.2087659465875866e+40,
	-1.2353681279003194e+40,
	-1.2624811783866065e+40,
	-1.2901134842995954e+40,
	-1.3182735463016053e+40,
	-1.3469699807093909e+40,
	-1.376211520749552e+40,
	-1.4060070178241288e+40,
	-1.4363654427864508e+40,
	-1.467295887227283e+40,
	-1.4988075647713377e+40,
	-1.53090981238419e+40,
	-1.5636120916896745e+40,
	-1.5969239902977956e+40,
	-1.6308552231432288e+40,
	-1.6654156338344522e+40,
	-1.700615196013573e+40,
	-1.7364640147269075e+40,
	-1.772972327806361e+40,
	-1.8101505072616751e+40,
	-1.848009060683592e+40,
	-1.8865586326579998e+40,
	-1.925810006191104e+40,
	-1.965774104145697e+40,
	-2.0064619906885721e+40,
	-2.0478848727491434e+40,
	-2.0900541014893326e+40,
	-2.1329811737847717e+40,
	-2.1766777337173903e+40,
	-2.221155574079436e+40,
	-2.2664266378889946e+40,
	-2.312503019917061e+40,
	-2.3593969682262275e+40,
	-2.407120885721035e+40,
	-2.4556873317100654e+40,
	-2.5051090234798155e+40,
	-2.555398837880421e+40,
	-2.606569812923291e+40,
	-2.658635149390713e+40,
	-2.7116082124574675e+40,
	-2.765502533324554e+40,
	-2.8203318108650453e+40,
	-2.8761099132821585e+40,
	-2.932850879779585e+40,
	-2.9905689222441536e+40,
	-3.049278426940882e+40,
	-3.1089939562204704e+40,
	-3.169730250239309e+40,
	-3.2315022286920596e+40,
	-3.2943249925568535e+40,
	-3.3582138258532056e+40,
	-3.423184197412655e+40,
	-3.489251762662244e+40,
	-3.5564323654208623e+40,
	-3.6247420397085334e+40,
	-3.6941970115686986e+40,
	-3.7648137009035717e+40,
	-3.8366087233226085e+40,
	-3.9095988920041743e+40,
	-3.983801219570454e+40,
	-4.0592329199756813e+40,
	-4.135911410407738e+40,
	-4.213854313203198e+40,
	-4.293079457775865e+40,
	-4.37360488255889e+40,
	-4.455448836960501e+40,
	-4.538629783333436e+40,
	-4.6231663989581335e+40,
	-4.709077578039724e+40,
	-4.796382433718948e+40,
	-4.885100300096964e+40,
	-4.975250734274223e+40,
	-5.06685351840339e+40,
	-5.159928661756414e+40,
	-5.254496402805806e+40,
	-5.35057721132019e+40,
	-5.448191790474193e+40,
	-5.547361078972727e+40,
	-5.648106253189749e+40,
	-5.750448729321545e+40,
	-5.854410165554627e+40,
	-5.960012464248279e+40,
	-6.067277774131828e+40,
	-6.176228492516753e+40,
	-6.28688726752359e+40,
	-6.399277000323835e+40,
	-6.51342084739679e+40,
	-6.6293422228015e+40,
	-6.747064800463817e+40,
	-6.866612516478644e+40,
	-6.988009571427467e+40,
	-7.111280432711194e+40,
	-7.236449836898409e+40,
	-7.363542792089088e+40,
	-7.492584580293842e+40,
	-7.623600759828776e+40,
	-7.756617167726001e+40,
	-7.891659922159886e+40,
	-8.028755424889141e+40,
	-8.167930363714742e+40,
	-8.309211714953811e+40,
	-8.452626745929527e+40,
	-8.598203017477079e+40,
	-8.745968386465811e+40,
	-8.89595100833756e+40,
	-9.048179339661288e+40,
	-9.202682140704089e+40,
	-9.359488478018583e+40,
	-9.518627727046869e+40,
	-9.680129574740986e+40,
	-9.844024022200048e+40,
	-1.0010341387324082e+41,
	-1.0179112307484625e+41,
	-1.0350367742212213e+41,
	-1.0524138975900729e+41,
	-1.070045762052881e+41,
	-1.0879355618398256e+41,
	-1.1060865244889608e+41,
	-1.1245019111234915e+41,
	-1.14318501673078e+41,
	-1.1621391704430832e+41,
	-1.1813677358200355e+41,
	-1.2008741111328796e+41,
	-1.2206617296504528e+41,
	-1.2407340599269379e+41,
	-1.2610946060913835e+41,
	-1.2817469081390057e+41,
	-1.30269454222427e+41,
	-1.3239411209557705e+41,
	-1.3454902936929057e+41,
	-1.3673457468443636e+41,
	-1.3895112041684199e+41,
	-1.4119904270750585e+41,
	-1.4347872149299226e+41,
	-1.4579054053600986e+41,
	-1.481348874561749e+41,
	-1.5051215376095923e+41,
	-1.5292273487682444e+41,
	-1.5536703018054245e+41,
	-1.5784544303070349e+41,
	-1.6035838079941241e+41,
	-1.6290625490417358e+41,
	-1.6548948083996529e+41,
	-1.6810847821150497e+41,
	-1.7076367076570512e+41
};
#endif

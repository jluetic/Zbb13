#include "lepSel.h"

selection getLepSel(std::string lepSel){
	if (lepSel=="DMu") return DMu;
	else if (lepSel=="EMu") return EMu;
	else if (lepSel=="DE") return DE;
	else if (lepSel=="SE") return SE;
	else if (lepSel=="SMu") return SMu;
	else {
		std::cout << "Error: no valid lepton selection" << std::endl;
		abort();
	}
}

TString getLepSel_string(selection lepSel){
	if (lepSel==DMu) return "DMu";
	else if (lepSel==EMu) return "EMu";
	else if (lepSel==DE) return "DE";
	else if (lepSel==SE) return "SE";
	else if (lepSel==SMu) return "SMu";
	else {
		std::cout << "Error: no valid lepton selection" << std::endl;
		abort();
	}
}



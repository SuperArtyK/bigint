
#include <iostream>
#include "AETimer.hpp"
#include "include/AEUtils.hpp"
#include "include/AEMath.hpp"

int main() {
#if ENGINE_ENABLE_GLOBAL_MODULES


	AETimer mytm(ENGINE_FPS);
	std::cout << mytm.getModuleAmount() << " " << mytm.getModuleName() << NLC;

	//AETimer::waitForTick(globalTimer)

	double i = 0.1;

	ullint iData[2]{};
	double fData[6]{};

	while (1) {
		AETimer::waitTicks(mytm, 1);

		iData[0] = mytm.getTick();
		iData[1] = ace::globalTimer.getTick();

		fData[0] = mytm.getWorldTime();
		fData[1] = mytm.getTPS();
		fData[2] = ace::globalTimer.getWorldTime();
		fData[3] = ace::globalTimer.getTPS();
		fData[4] = (fData[0] - fData[2]) / fData[2] * 100.0;
		fData[5] = (fData[5] * (i * 2) + fData[4]) / (i * 2 + 1);


		std::printf("Ticks (mytm)  : %6llu; Time (mytm)  : %6.3f; TPS (mytm)  : %6.3f \n"
			"Ticks (global): %6llu; Time (global): %6.3f; TPS (global): %6.3f \n"
			"Current drift: %6.3f%%; Average drift: %6.3f%%\n", 
			iData[0], fData[0], fData[1],
			iData[1], fData[2], fData[3],
			fData[4], fData[5]);


		//std::cout << "Ticks (mytm)  : " << mytm.getTick() << "; Time (mytm): " << mytm.getWorldTime() << "; TPS (mytm): " << mytm.getTPS() << NLC;
		//std::cout << "Ticks (global): " << ace::globalTimer.getTick() << "; Time (global): " << ace::globalTimer.getWorldTime() << "; TPS (global): " << ace::globalTimer.getTPS() << NLC<<NLC;

		mytm.setTPS(ace::math::absval(ace::math::sinDeg(i += 0.5)) * (ENGINE_FPS * 2));
		//ace::globalTimer.setTPS(10);
	}



#endif // ENGINE_ENABLE_GLOBAL_MODULES
}

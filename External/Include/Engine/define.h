#pragma once

#define SINGLE(classtype)  	private:\
								classtype();\
								~classtype();\
							friend class CSingleton<classtype>;
#pragma once

//材质
struct stMaterial
{
	stMaterial()
	{
		emissiveR = emissiveG = emissiveB = emissiveA = 0;		//自发光
		ambientR = ambientG = ambientB = ambientA =1;			//环境光
		diffuseR = diffuseG = diffuseB = diffuseA =1;			//漫射光
		specularR = specularG = specularB = specularA = 0;		//镜面光
		

		power = 0;
	}

	float ambientR, ambientG, ambientB, ambientA;			//环境光
	float diffuseR, diffuseG, diffuseB, diffuseA;			//漫射光
	float specularR, specularG, specularB, specularA;		//镜面光
	float emissiveR, emissiveG, emissiveB, emissiveA;		//自发光

	float power;
};


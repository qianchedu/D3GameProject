#pragma once

//����
struct stMaterial
{
	stMaterial()
	{
		emissiveR = emissiveG = emissiveB = emissiveA = 0;		//�Է���
		ambientR = ambientG = ambientB = ambientA =1;			//������
		diffuseR = diffuseG = diffuseB = diffuseA =1;			//�����
		specularR = specularG = specularB = specularA = 0;		//�����
		

		power = 0;
	}

	float ambientR, ambientG, ambientB, ambientA;			//������
	float diffuseR, diffuseG, diffuseB, diffuseA;			//�����
	float specularR, specularG, specularB, specularA;		//�����
	float emissiveR, emissiveG, emissiveB, emissiveA;		//�Է���

	float power;
};


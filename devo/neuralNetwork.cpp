#ifndef _NEURAL_NETWORK_CPP
#define _NEURAL_NETWORK_CPP

#include "iostream"

#include "neuralNetwork.h"

#include "math.h"

extern int MAX_NEURONS;

extern int MAX_SYNAPSES;

extern int SENSOR_NEURON;

extern int BIAS_NEURON;

extern int HIDDEN_NEURON;

extern int MOTOR_NEURON;

NEURAL_NETWORK::NEURAL_NETWORK(void) {

	Initialize_Neurons();

	Initialize_Synapses();
}

NEURAL_NETWORK::~NEURAL_NETWORK(void) {

}

void NEURAL_NETWORK::Add_Bias_Neuron(int ID) {

        neurons[numNeurons] = new NEURON(ID,BIAS_NEURON,1.0);

        numNeurons++;

}

void NEURAL_NETWORK::Add_Hidden_Neuron(int ID, double tau) {

	neurons[numNeurons] = new NEURON(ID,HIDDEN_NEURON,tau);

	numNeurons++;
}

NEURON *NEURAL_NETWORK::Add_Motor_Neuron(int ID, double tau) {

        NEURON *newNeuron = new NEURON(ID,MOTOR_NEURON,tau);

        neurons[numNeurons] = newNeuron;

	numNeurons++;

        return newNeuron;
}

NEURON *NEURAL_NETWORK::Add_Sensor_Neuron(int ID, int svIndex, double tau) {

	NEURON *newNeuron = new NEURON(ID,SENSOR_NEURON,svIndex,tau);

	neurons[numNeurons] = newNeuron;

	numNeurons++;

	return newNeuron;
}

void NEURAL_NETWORK::Add_Synapse(void) {

        synapses[numSynapses++] = new SYNAPSE(); 
}

void NEURAL_NETWORK::Update(int t) {

	Push_Current_Values_To_Previous_Values();

	Reset_Neuron_Values();

	Update_Neurons(t);

	Threshold_Neurons();
}

// ------------------------- Private methods -----------------------------

void NEURAL_NETWORK::Initialize_Neurons(void) {

        neurons = new NEURON * [MAX_NEURONS];

        for (int n = 0 ; n < MAX_NEURONS ; n++ )

                neurons[n] = NULL;

        numNeurons = 0;
}

void NEURAL_NETWORK::Initialize_Synapses(void) {

        synapses = new SYNAPSE * [MAX_SYNAPSES];

        for (int s = 0 ; s < MAX_SYNAPSES ; s++ )

                synapses[s] = NULL;

        numSynapses = 0;
}

void NEURAL_NETWORK::Push_Current_Values_To_Previous_Values(void) {

        for (int n = 0 ; n < numNeurons ; n++ )

		neurons[n]->Push_Current_Value_To_Previous_Value();
}

void NEURAL_NETWORK::Reset_Neuron_Values(void) {

	for ( int n = 0 ; n < numNeurons ; n++ )

		neurons[n]->Reset();
}

void NEURAL_NETWORK::Threshold_Neurons(void) {

        for ( int n = 0 ; n < numNeurons ; n++ )

		neurons[n]->Threshold();	
}

void NEURAL_NETWORK::Update_Neurons(int t) {

	for ( int s = 0 ; s < numSynapses ; s++ ) {

		int sni = synapses[s]->Get_Source_Neuron_Index();

		int tni = synapses[s]->Get_Target_Neuron_Index();
	
		double w = synapses[s]->Get_Weight(t);

		double influence = w * neurons[sni]->Get_Previous_Value();

		neurons[tni]->Set( neurons[tni]->Get_Value() + influence );
	}

}

#endif

#include "plugin.hpp"


struct Mixer : Module {
	enum ParamIds {
		CH1_PARAM,
		CH2_PARAM,
		CH3_PARAM,
		CH4_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		IN1_INPUT,
		IN2_INPUT,
		IN3_INPUT,
		IN4_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		OUT1_OUTPUT,
		OUT2_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		OUT_POS_LIGHT,
		OUT_NEG_LIGHT,
		NUM_LIGHTS
	};

	Mixer() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(CH1_PARAM, 0.0, 1.0, 0.0, "Ch 1 level", "%", 0, 100);
		configParam(CH2_PARAM, 0.0, 1.0, 0.0, "Ch 2 level", "%", 0, 100);
		configParam(CH3_PARAM, 0.0, 1.0, 0.0, "Ch 3 level", "%", 0, 100);
		configParam(CH4_PARAM, 0.0, 1.0, 0.0, "Ch 4 level", "%", 0, 100);
	}

	void process(const ProcessArgs &args) override {
		float in1 = inputs[IN1_INPUT].getVoltage() * params[CH1_PARAM].getValue();
		float in2 = inputs[IN2_INPUT].getVoltage() * params[CH2_PARAM].getValue();
		float in3 = inputs[IN3_INPUT].getVoltage() * params[CH3_PARAM].getValue();
		float in4 = inputs[IN4_INPUT].getVoltage() * params[CH4_PARAM].getValue();

		float out = in1 + in2 + in3 + in4;
		outputs[OUT1_OUTPUT].setVoltage(out);
		outputs[OUT2_OUTPUT].setVoltage(-out);
		lights[OUT_POS_LIGHT].setSmoothBrightness(out / 5.f, args.sampleTime);
		lights[OUT_NEG_LIGHT].setSmoothBrightness(-out / 5.f, args.sampleTime);
	}
};




struct MixerWidget : ModuleWidget {
	MixerWidget(Mixer *module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Mixer.svg")));

		addChild(createWidget<Knurlie>(Vec(15, 0)));
		addChild(createWidget<Knurlie>(Vec(15, 365)));

		addParam(createParam<Davies1900hWhiteKnob>(Vec(19, 32), module, Mixer::CH1_PARAM));
		addParam(createParam<Davies1900hWhiteKnob>(Vec(19, 85), module, Mixer::CH2_PARAM));
		addParam(createParam<Davies1900hWhiteKnob>(Vec(19, 137), module, Mixer::CH3_PARAM));
		addParam(createParam<Davies1900hWhiteKnob>(Vec(19, 190), module, Mixer::CH4_PARAM));

		addInput(createInput<PJ301MPort>(Vec(7, 242), module, Mixer::IN1_INPUT));
		addInput(createInput<PJ301MPort>(Vec(43, 242), module, Mixer::IN2_INPUT));

		addInput(createInput<PJ301MPort>(Vec(7, 281), module, Mixer::IN3_INPUT));
		addInput(createInput<PJ301MPort>(Vec(43, 281), module, Mixer::IN4_INPUT));

		addOutput(createOutput<PJ301MPort>(Vec(7, 324), module, Mixer::OUT1_OUTPUT));
		addOutput(createOutput<PJ301MPort>(Vec(43, 324), module, Mixer::OUT2_OUTPUT));

		addChild(createLight<MediumLight<GreenRedLight>>(Vec(32.7, 310), module, Mixer::OUT_POS_LIGHT));
	}
};


Model *modelMixer = createModel<Mixer, MixerWidget>("Mixer");

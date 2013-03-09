/*
==============================================================================

  This file was auto-generated by the Introjucer!

  It contains the basic startup code for a Juce application.

==============================================================================
*/

#ifndef __PLUGINEDITOR_H_8FBA87C0__
#define __PLUGINEDITOR_H_8FBA87C0__

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class LuftikusLookAndFeel : public LookAndFeel
{
public:
	LuftikusLookAndFeel()
	{
		setColour(TextEditor::focusedOutlineColourId, Colours::transparentBlack);
		setColour(TextButton::buttonColourId, Colours::black);
		setColour(TextButton::buttonOnColourId, Colours::black);
		setColour(ComboBox::buttonColourId, Colours::darkgrey);
		setColour(Slider::trackColourId, Colours::black);
		setColour(Slider::rotarySliderFillColourId, Colours::black);
		setColour(Slider::rotarySliderOutlineColourId, Colours::black);
		setColour(Slider::textBoxBackgroundColourId, Colours::white.withAlpha(0.5f));
		setColour(Slider::textBoxHighlightColourId, Colours::lightgrey);

		setColour(ComboBox::backgroundColourId, Colours::white.withAlpha(0.5f));
	}

	void drawTickBox (Graphics& g,
                              Component& component,
                              float x, float y, float w, float h,
                              bool ticked,
                              bool isEnabled,
                              bool /*isMouseOverButton*/,
                              bool /*isButtonDown*/)
	{
		g.setColour(component.findColour (TextButton::buttonColourId).withMultipliedAlpha (isEnabled ? 1.0f : 0.5f));
		g.drawRect(x+w*0.15f, y+h*0.15f, w*0.7f, h*0.7f);

		if (ticked)
		{
			g.drawLine(x+w*0.15f, y+h*0.15f, x+w*0.85f, y+h*0.85f);
			g.drawLine(x+w*0.85f, y+h*0.15f, x+w*0.15f, y+h*0.85f);
		}
	}


	void drawComboBox (Graphics& g, int width, int height,
									bool /*isButtonDown*/,
									int buttonX, int buttonY,
									int buttonW, int buttonH,
									ComboBox& box)
	{
		g.fillAll (box.findColour (ComboBox::backgroundColourId));

		if (box.isEnabled() && box.hasKeyboardFocus (false))
		{
			g.setColour (box.findColour (ComboBox::buttonColourId));
			g.drawRect (0, 0, width, height, 2);
		}
		else
		{
			g.setColour (box.findColour (ComboBox::outlineColourId));
			g.drawRect (0, 0, width, height);
		}

		//const float outlineThickness = box.isEnabled() ? (isButtonDown ? 1.2f : 0.5f) : 0.3f;

		const Colour baseColour (box.findColour (ComboBox::buttonColourId).withMultipliedAlpha (box.isEnabled() ? 1.0f : 0.5f));

		//drawGlassLozenge (g,
		//				  buttonX + outlineThickness, buttonY + outlineThickness,
		//				  buttonW - outlineThickness * 2.0f, buttonH - outlineThickness * 2.0f,
		//				  baseColour, outlineThickness, -1.0f,
		//				  true, true, true, true);

		if (box.isEnabled())
		{
			const float arrowX = 0.3f;
			const float arrowH = 0.2f;

			Path p;
			p.addTriangle (buttonX + buttonW * 0.5f,            buttonY + buttonH * (0.45f - arrowH),
						   buttonX + buttonW * (1.0f - arrowX), buttonY + buttonH * 0.45f,
						   buttonX + buttonW * arrowX,          buttonY + buttonH * 0.45f);

			p.addTriangle (buttonX + buttonW * 0.5f,            buttonY + buttonH * (0.55f + arrowH),
						   buttonX + buttonW * (1.0f - arrowX), buttonY + buttonH * 0.55f,
						   buttonX + buttonW * arrowX,          buttonY + buttonH * 0.55f);

			g.setColour (box.findColour (ComboBox::arrowColourId));
			g.fillPath (p);
		}
	}


	void drawRotarySlider (Graphics& g,
                           int x, int y,
                           int width, int height,
                           float sliderPos,
                           float rotaryStartAngle,
                           float rotaryEndAngle,
                           Slider& slider)
	{
		const float radius = jmin (width / 2, height / 2) - 2.0f;
		const float centreX = x + width * 0.5f;
		const float centreY = y + height * 0.5f;
		const float rw = radius * 2.0f;
		const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
		const bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();

		{
			Colour c(slider.isEnabled() ? slider.findColour (Slider::rotarySliderFillColourId).withAlpha (isMouseOver ? 0.9f : 0.7f) : Colour(0x80808080));

			AffineTransform transform(AffineTransform::rotation (angle).translated (centreX, centreY));

			g.setColour(c);
			Path p;
			p.addEllipse (-0.4f * rw, -0.4f * rw, rw * 0.8f, rw * 0.8f);
			p.addPolygon(Point<float> (0.f, 0.f), 8, 0.4f*rw);
			g.setColour(c.brighter(0.2f));
			g.fillPath(p, transform);

			PathStrokeType(jmin(2.5f, rw * 0.1f)).createStrokedPath (p, p);

			p.addLineSegment (Line<float> (0.f, -rw*0.15f, 0.0f, -rw*0.4f), jmin(2.5f, rw * 0.2f));

			g.setColour(c);
			g.fillPath (p, transform);
		}
	}

private:

};

class XSlider : public Slider
{
public:

	XSlider() : Slider(), resetValue(0) {}
	explicit XSlider (const String& componentName) : Slider(componentName), resetValue(0) {}
	XSlider (SliderStyle style, TextEntryBoxPosition textBoxPosition) : Slider(style, textBoxPosition), resetValue(0) {}

    void mouseDoubleClick (const MouseEvent& /*ev*/)
	{
		setValue(resetValue, sendNotificationAsync);
	}
	
private:

	double resetValue;


};

//==============================================================================
/**
*/
class LuftikusAudioProcessorEditor  : public AudioProcessorEditor,
	                                  public Slider::Listener,
									  public ComboBox::Listener,
									  public Timer,
									  public Button::Listener
{
public:
  LuftikusAudioProcessorEditor (LuftikusAudioProcessor* ownerFilter);
  ~LuftikusAudioProcessorEditor();

  //==============================================================================
  // This is just a standard Juce paint method...
	void resized();
	void paint (Graphics& g);

	void timerCallback();

	void sliderValueChanged (Slider* slider);
	void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
	void buttonClicked (Button* button);


private:
	void updateSliders();
	void updateTooltipState();

	LuftikusAudioProcessor* Proc;

	XSlider sliders[EqDsp::kNumTypes];
	Label labels[EqDsp::kNumTypes];
	ComboBox type;
	ToggleButton mastering;
	ToggleButton analog;
	ToggleButton keepGain;

	ToggleButton showTooltips;

	ToggleButton types[EqDsp::kNumHighSelves];

	XSlider masterVol;
	Label masterVolLabel;

	Image background;
	Image luftikus;

	LuftikusLookAndFeel lookAndFeel; 

	ScopedPointer<TooltipWindow> tooltips;
};


#endif  // __PLUGINEDITOR_H_8FBA87C0__
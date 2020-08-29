#pragma once

#include <JuceHeader.h>
#include <stdio.h>

class DragNode : public juce::Component
{
public:
    DragNode()
    {
        int initSideLength = getParentHeight();
        sideLength = initSideLength;
        setSize(sideLength, sideLength);
        float parentSideRatio = getParentWidth() / getParentHeight();
        setBoundsRelative(0.0f, 0.0f, parentSideRatio, 1.0f);
        juce::Rectangle<int> bounds = getBoundsInParent().reduced(5);
        constrainer.setSizeLimits(bounds.getY(),
                                  bounds.getX(),
                                  bounds.getWidth(),
                                  bounds.getHeight());
        constrainer.setMinimumOnscreenAmounts(0xffffff, 0xffffff, 0xffffff, 0xffffff);
        setTopLeftPosition(0, 0);
    }
    ~DragNode() override
    {
    }
    void moved() override
    {
    
    }
    int getCenterX()
    {
        return getX() + (sideLength / 2);
    }
    int getCenterY()
    {
        return getY() + (sideLength / 2);
    }
    void resized() override
    {
        sideLength = getParentHeight();
        setSize(sideLength, sideLength);
    }
    void paint(juce::Graphics& g) override
    {
        g.fillAll(setColor);
    }
    void mouseDown(const juce::MouseEvent &event) override
    {
        dragger.startDraggingComponent(this, event);
    }
    void mouseDrag(const juce::MouseEvent &event) override
    {
    dragger.dragComponent(this, event, &constrainer);
    }
    void assignColor(juce::Colour inputColor)
    {
        setColor = inputColor;
    }
    
private:
    bool firstDragLoop = true;
    int sideLength;
    juce::ComponentDragger dragger;
    juce::ComponentBoundsConstrainer constrainer;
    juce::Colour setColor = juce::Colours::blue;
};

class NodeWatcher : public juce::ComponentMovementWatcher
{
public:
    NodeWatcher(DragNode* chosenNode) : juce::ComponentMovementWatcher(chosenNode)
    {
        nodeToWatch = chosenNode;
    }
    ~NodeWatcher() {};
    void componentMovedOrResized(juce::Component &component, bool wasMoved, bool wasResized) override
    {
        juce::Rectangle<int> targetBounds = component.getBounds();
        int width = targetBounds.getWidth();
        watchedXLeft = component.getX();
        watchedYTop = component.getY();
        
        watchedXCenter = component.getX() + (width / 2);
        watchedYCenter = component.getY() + (width / 2);
        
        watchedXRight = component.getX() + width;
        watchedYBottom = component.getY() + width;
    }
    int watchedXLeft;
    int watchedYTop;
    int watchedXCenter;
    int watchedYCenter;
    int watchedXRight;
    int watchedYBottom;
private:
    DragNode* nodeToWatch;
};

class DragNodeContainer : public juce::Component
{
public:
    DragNodeContainer(int ix, int iy, int iwidth, int iheight)
    {
        addAndMakeVisible(dragNode);
        x = ix;
        y = iy;
        width = iwidth;
        height = iheight;
        setBounds(x, y, width, height);
        dragNode.setBounds(x, y, height, height);
        dragNode.setTopLeftPosition(0, 0);
    }
    ~DragNodeContainer()
    {}
    void reInit(int ix, int iy, int iwidth, int iheight)
    {
        x = ix;
        y = iy;
        width = iwidth;
        height = iheight;
        setBounds(x, y, width, height);
        dragNode.setBounds(x, y, height, height);
        dragNode.setTopLeftPosition(0, 0);
        setTopLeftPosition(x, y);
    }
    void initNodePlacement()
    {
        dragNode.setBounds(getX(), getY(), getHeight(), getHeight());
        dragNode.setTopLeftPosition(0, 0);
    }
    void setNodeColor(juce::Colour inputColor)
    {
        dragNode.assignColor(inputColor);
    }
    void paint(juce::Graphics &g) override
    {
        g.fillAll(juce::Colours::white);
    }
    int getXSetting()
    {
        printf ("X set to: %d\n", dragNode.getCenterX() - x);
        return (dragNode.getCenterX() - x);
    }
    DragNode dragNode;
private:
    int xOutput;
    int x, y, width, height;
};







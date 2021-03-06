#pragma once
#include <JuceHeader.h>

#include <stdio.h>

enum direction{hor, vert};
//===========================================
class DragPoint : public juce::Component
{
public:
    DragPoint(int height)
    {
        int initSideLength = height;
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
        updateReturnPoints();
        
    }
    ~DragPoint() override
    {}
    void resized() override
    {
        setSize(sideLength, sideLength);
    }
    void paint(juce::Graphics& g) override
    {
        g.fillAll(setColor);
    }
    void updateReturnPoints()
    {
        juce::Rectangle<int> updatedBounds = getBoundsInParent();
        leftX = updatedBounds.getX();
        topY = updatedBounds.getY();
        centerX = leftX + (updatedBounds.getWidth() / 2);
        centerY = topY + (updatedBounds.getHeight() / 2);
        bottomY = topY + updatedBounds.getHeight();
        rightX = leftX + updatedBounds.getWidth();
    }
    void updateCenterX() {centerX = getX() + (getHeight() / 2);}
    void updateCenterY() {centerY = getY() + (getHeight() / 2);}
    void mouseDown(const juce::MouseEvent &event) override
    {
        dragger.startDraggingComponent(this, event);
        
    }
    void mouseDrag(const juce::MouseEvent &event) override
    {
        dragger.dragComponent(this, event, &constrainer);
        updateReturnPoints();
    }
    void assignColor(juce::Colour inputColor)
    {
        setColor = inputColor;
    }
    int centerX, centerY, leftX, rightX, topY, bottomY;
    juce::Colour setColor = juce::Colours::blue;
private:
    bool firstDragLoop = true;
    int sideLength;
    juce::ComponentDragger dragger;
    juce::ComponentBoundsConstrainer constrainer;
};
//=====================================================
enum side {top, right, bottom, left};

const char* getSideName(side enumVal)
{
    switch(enumVal)
    {
        case top:
            return "top";
        case right:
            return "right";
        case bottom:
            return "bottom";
        case left:
            return "left";
    }
}

//===================================================
class DraggerContainer : public juce::Component,
public juce::ComponentListener
{
public:
    DraggerContainer(int xMinSet, int yMinSet, int widthSet, int heightSet, int dragPointSide) : point(dragPointSide)
    {
        addAndMakeVisible(point);
        contMinX = xMinSet;
        contMaxX = xMinSet + widthSet;
        contMinY = yMinSet;
        contMaxY = yMinSet + heightSet;
        contWidth = widthSet;
        contHeight = heightSet;
        dragPointSize = dragPointSide;
        setBounds(contMinX, contMinY, contWidth, contHeight);
        point.setBounds(contMinX, contMinY, dragPointSide, dragPointSide);
        point.setTopLeftPosition(0, 0);
      
    }
    ~DraggerContainer()
    {}
    
    void setChildColor(juce::Colour colorChoice)
    {
        point.assignColor(colorChoice);
    }
    //======functions for movement constraint
    void addPeer(DraggerContainer* peer, side limitSide)
    {
        limitingPoints.push_back(peerPoint(peer, limitSide));
        peer->point.addComponentListener(this);
    }
    void componentMovedOrResized(juce::Component& component, bool wasMoved, bool wasResized) override
    {
        //printf("Peer DragPoint moved\n");
        checkLimitUpdates();
    }
    void paint(juce::Graphics &g) override
    {
        g.setColour(point.setColor);
        juce::Rectangle<int> toDraw = getLocalBounds();
        g.drawRect(toDraw, 1);
    }
    
    void resetPointHome(int newX, int newY)
    {
        dragPointHome = juce::Point<int>(newX, newY);
        point.setTopLeftPosition(dragPointHome);
    }
    
    void checkLimitUpdates()
    {
        for(int i = 0; i < limitingPoints.size(); ++i)
        {
            auto fromSide = limitingPoints[i].sourceSide;
            limitingPoints[i].sourceContainer->point.updateReturnPoints();
            int lastDimSetting;
            int* constrainedDim;
            switch(fromSide)
            {
                case top: // minY & height are limited
                    constrainedDim = limitingPoints[i].minYSource;
                    lastDimSetting = limitingPoints[i].lastMinY;
                    break;
                case right:
                    constrainedDim = limitingPoints[i].maxXSource;
                    lastDimSetting = limitingPoints[i].lastMaxX;
                    break;
                case bottom:
                    constrainedDim = limitingPoints[i].maxYSource;
                    lastDimSetting = limitingPoints[i].lastMaxY;
                    break;
                case left:
                    constrainedDim = limitingPoints[i].minXSource;
                    lastDimSetting = limitingPoints[i].lastMinX;
                    break;
            }
            if(lastDimSetting != *constrainedDim)
            {
                int newPos = *constrainedDim;
                //these will be the arguments to the call to setBounds() at the end of this function
                //REMEMBER: getX() and getY() return the DIFFERENCE between the component's minimum and its parent's minimum
                int destMinX = getX();
                int destMinY = getY();
                int destMaxX = getRight();
                int destMaxY = getBottom();
                int destWidth = getWidth();
                int destHeight = getHeight();
                //the switch changes the value of one of the above ints to the cooresponding value from the peerPoint
                //note: width or height also needs to be updated each time a destMin/Max is changed
                switch(fromSide)
                {
                    case top:
                        destMinY = newPos;
                        /*minimum values need to account for the intitial position of the sourceContainer, or else they'll be offset from the screen's origin rather than the sourceContainer's origin*/
                        destMinY += limitingPoints[i].sourceContainer->contMinY;
                        destHeight = destMaxY - destMinY;
                        break;
                    case right:
                        destMaxX = newPos;
                        printf("newRightLimit: %d\n", newPos);
                        //destMaxX += limitingPoints[i].sourceContainer->contMaxX;
                        destWidth = destMaxX - destMinX;
                        break;
                    case bottom:
                        destMaxY = newPos;
                        destHeight = destMaxY - destMinY;
                        break;
                    case left:
                        destMinX = newPos;
                        int limitDim = limitingPoints[i].sourceContainer->contMinX;
                        if((limitDim + newPos) < contMaxX)
                            destMinX += limitDim;
                        destWidth = destMaxX - destMinX;
                        break;
                }
                setBounds(destMinX, destMinY, destWidth, destHeight);
            }
            limitingPoints[i].updateLastPoints();
        }
    }
    // public data member
    DragPoint point;
private:
    juce::Point<int> dragPointHome = juce::Point<int>(0, 0);
    DraggerContainer* peerCont1;
    int contWidth, contHeight, contMinX, contMaxX, contMinY, contMaxY; //all the dimension points
    int dragPointSize;
    int childX, childY; //top left corner of the DragPoint
    class peerPoint
    {
        public:
        peerPoint(DraggerContainer* source, side onSide)
        {
            sourceContainer = source;
            sourceSide = onSide;
            minXSource = &sourceContainer->point.leftX;
            minYSource = &sourceContainer->point.topY;
            maxXSource = &sourceContainer->point.rightX;
            maxYSource = &sourceContainer->point.bottomY;
        }
        ~peerPoint() {}
        void updateLastPoints()
        {
            //sourceContainer->point.updateReturnPoints();
            lastMinX = *minXSource;
            lastMinY = *minYSource;
            lastMaxX = *maxXSource;
            lastMaxY = *maxYSource;
        }
        //data members
        DraggerContainer* sourceContainer;
        int* minXSource;
        int* maxXSource;
        int* minYSource;
        int* maxYSource;
        int* valToConstrain;
        int lastMinX, lastMaxX, lastMinY, lastMaxY;
        side sourceSide;
    };
    
    std::vector<peerPoint> limitingPoints;
};



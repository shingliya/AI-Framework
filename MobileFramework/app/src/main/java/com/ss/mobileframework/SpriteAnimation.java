package com.ss.mobileframework;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Rect;

import com.ss.mobileframework.Utility.Vector2;

/**
 * Created by 142128G on 12/3/2015.
 */
public class SpriteAnimation
{
    private Bitmap bitmap;      //Animation Sequence
    private Rect sourceRect;    //Rectangular to be drawn from the animation bitmap
    private int frame;          //Number of frames in animation
    private int currentFrame;   //Current Frame
    private long frameTicker;
    private int framePeriod;

    private int spriteWidth;
    private int spriteHeight;

    private Vector2 pos;

    public SpriteAnimation(Bitmap bitmap, int x, int y, int fps, int frameCount)
    {
        this.bitmap = bitmap;

        pos = new Vector2();
        pos.x = x;
        pos.y = y;

        currentFrame = 0;

        frame = frameCount;

        spriteWidth = bitmap.getWidth() / frameCount;
        spriteHeight = bitmap.getHeight();

        sourceRect = new Rect(0, 0, spriteWidth, spriteHeight);

        framePeriod = 1000 / fps;
        frameTicker = 0;
    }

    public Bitmap getBitmap()
    {
        return bitmap;
    }

    public void setBitmap(Bitmap bitmap)
    {
        this.bitmap = bitmap;
    }

    public Rect getSourceRect()
    {
        return sourceRect;
    }

    public void setSourceRect(Rect sourceRect)
    {
        this.sourceRect = sourceRect;
    }

    public int getFrame()
    {
        return frame;
    }

    public void setFrame(int frame)
    {
        this.frame = frame;
    }

    public int getCurrentFrame()
    {
        return currentFrame;
    }

    public void setCurrentFrame(int currentFrame)
    {
        this.currentFrame = currentFrame;
    }

    public int getFramePeriod()
    {
        return framePeriod;
    }

    public void setFramePeriod(int framePeriod)
    {
        this.framePeriod = framePeriod;
    }

    public int getSpriteWidth()
    {
        return spriteWidth;
    }

    public void setSpriteWidth(int spriteWidth)
    {
        this.spriteWidth = spriteWidth;
    }
    public int getSpriteHeight()
    {
        return spriteHeight;
    }

    public void setSpriteHeight(int spriteHeight)
    {
        this.spriteHeight = spriteHeight;
    }

    public Vector2 getPos()
    {
        return pos;
    }

    public void setX(int x)
    {
        pos.x = x;
    }

    public void setY(int y)
    {
        pos.y = y;
    }

    public void update(long dt)
    {
        if(dt > frameTicker + framePeriod)
        {
            frameTicker = dt;
            currentFrame++;

            if(currentFrame >= frame)
            {
                currentFrame = 0;
            }
        }

        //define the rect to cut out sprite
        this.sourceRect.left = currentFrame * spriteWidth;
        this.sourceRect.right = this.sourceRect.left + spriteWidth;
    }

    public void draw(Canvas canvas)
    {
        Rect destRect = new Rect(Math.round(getPos().x), Math.round(getPos().y), Math.round(getPos().x) + spriteWidth, Math.round(getPos().y) + spriteHeight);
        canvas.drawBitmap(bitmap, sourceRect, destRect, null);
    }
}

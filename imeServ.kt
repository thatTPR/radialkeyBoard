package com.example

import android.inputmethodservice.InputMethodService
import android.view.View
import android.widget.Button
import android.widget.LinearLayout

class RadialIme : InputMethodService() {
    external fun nativeInit(  surface: Surface , assetManager: AssetManager)
    external fun nativeup(id:Int,  x: Float, y: Float)
    external fun nativedown(id:Int,  x: Float, y: Float)
    external fun nativemove(id:Int,  x: Float, y: Float)

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate()
        System.loadLibrary("keyboard")
        
        val am = assets 
        val sv = SurfaceView(this) 

        nativeInit(sv,am)
    }

    override fun onCreateInputView(): View {

        val view = View(this) ;

        view.setOnTouchListener { _, event ->

            
            when (event.actionMasked) {
              
        MotionEvent.ACTION_DOWN -> {

            for (i in 0 until event.pointerCount) {
                val id = event.getPointerId(i)

                nativedown(id, event.getX(i), event.getY(i))
            }
        }

        MotionEvent.ACTION_MOVE -> {
            for (i in 0 until event.pointerCount) {
                val id = event.getPointerId(i)

                Int i =  nativemove(id, event.getX(i), event.getY(i)) 
                if(i!=0){
                    val et = currentInputConnection.getExtractedText(
       ExtractedTextRequest(),0) 
                    currentInputConnection.setSelection(et?.selectionStart+i ,et?.selectionStart+i ) ;
                }
            }
        }

        MotionEvent.ACTION_UP -> {
            for (i in 0 until event.pointerCount) {
                val id = event.getPointerId(i)
                String str = nativeup(id, event.getX(i), event.getY(i))
                if(str.length>0){currentInputConnection.commitText(str,1);};
            }
        }

    }

    true
  
        }

        return view
    }
    ovverride fun onDestroyInputView() : View {
        nativeClose();
        super.onDestroyInputView();

    }
}
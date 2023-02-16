// Notes: ABOUT NTOUCH_xxx and NTOUCHX_xxx Events
//
// N is suppose to help creating cross platform applications with as much as possible permit to avoid code redundancies.
// There are a lot of different pointing devices on all platforms, like touch screens, mouse, pen, trackball, track pad , ...
// All of these devices are doing almost the same ( ... with some subtle but important differences ).
// A simple way to manage them could be having specific events for each of them, like this:
//			NMOUSE_BUTTONDOWN,NMOUSE_BUTTONUP,NMOUSE_MOVE	... for mouse
//			NTOUCH_DOWN,NTOUCH_UP,NTOUCH_MOVE				... for touch screen
//			etc ...
// But in that way, user has to respond to all the different event if he wants to create an application that works on PC for instance ( with MOUSE )
// and mobile phone (with touch screen )
// So, if it seems simple on N Engine side it could complicate things for programmer who's just interested by the fact user "points" something
// ( with whatever he wants actually ! )
//
// N give an elegant solution to handle almost all programmer wishes.
// Everything is done regarding pointing device behavior instead of pointing device itself.
// In that way, an NTOUCH_??? event is posted by a pointing device when it behaves exactly like a touch screen. 
// And, when it behaves differently it posts an NTOUCHX_??? event ! ( ONLY IF pre-compiler tag '_NTOUCH_EXTENDED' is define. ) 
//
//				In that way, a programmer who wants to create an application fully compatible with all the pointing devices must only
//				handle the NTOUCH_??? Events ( for pointing device handling ).
//
// All the Event type ID share the same build scheme (obviously) which is: 
//				Eventtype_ID = Maintype_ID + SubType_ID 
//
//				NTOUCH_???  event are like this: _TOUCH  + subtype	where _TOUCH  is the main type
//				NTOUCHX_??? event are like this: _TOUCHX + subtype	where _TOUCHX is the main type
//				NTOUCH and NTOUCHX Subtypes are common : _START,_END,_MOVE and _CANCEL. ( _CANCEL should never happen with a mouse but in theroy it can ... )
//
// So programmer who wants to deal with all pointing device behaviors without isolating "touch screen behavior"(NTOUCH_)
// must deal with 'event_subtype' instead of 'event type'
//
// EXAMPLES:
//
// Use Event Type to 'isolate' NTOUCH behavior even if 'source' pointing device is not a 'touch screen'  
// 	switch( pevent->Type )
// 	{
// 		// NTOUCH_???. Send by pointing device when they behave like touch screen.
//		// UI And GameState receiving NTOUCH_??? Events !!!
//		//
// 		case NTOUCH_START:		//| NTOUCH_START is always the first event of a Touch gesture
// 		case NTOUCH_MOVE:		//|	After a gesture was 'started' with a NTOUCH_START. NTOUCH_MOVE is posted each time touch(es) change(s) ( position, pressure, etc ... ). 
// 		case NTOUCH_END:		//| NTOUCH_END is the last event of a Touch gesture
// 		case NTOUCH_CANCEL:		//| NTOUCH_CANCEL is the last event of a Touch gesture too but it means "loose the touch" and has to be handled in a diffrent way than NTOUCH_END..
// 
// 		// NTOUCHX_???. Send by pointing device when they behave differently than a touch screen.
//		// !!! Only GameStates receiving NTOUCHX_??? Events !!! 
//		// !!! NTOUCHX_??? events exist only if the pre-compilator tag '_TOUCH_EXTENDED' is define !!!
// 		case NTOUCHX_START:
// 		case NTOUCHX_MOVE:
// 		case NTOUCHX_END:
// 		case NTOUCHX_CANCEL:
// 	}
// 
// 	// Use Event SubType to get all pointer device behaviors  with no care of  NTOUCH and NTOUCHX
//	// In that use, NTOUCH and NTOUCHX plain type event event are going to be 'mixed'.
//	// Don't forget ... 
//	// !!! NTOUCHX_??? events exist only if the pre-compilator tag '_TOUCH_EXTENDED' is define !!!
//	// So, without this tag it will be exactly the same than the switch(pevent->Type) with NTOUCH_??? case.
// 	switch( NGET_EVENT_SUB(pevent->Type) )
// 	{
// 		case _START:
// 		case _MOVE:
// 		case _END:
// 		case _CANCEL:
// 	}
//
// !!! IMPORTANT !!!
// !!! MOUSE MOVE without any button pressed is not handle by N ENGINE !!!


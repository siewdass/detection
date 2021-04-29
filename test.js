const dlib  = require( './lib/detection' )

var minotaur = new dlib.objectDetector( 'assets/detector.svm' );

var start = new Date( ).getTime( )
var detections = minotaur.detector( 'assets/test.jpg' )
var end = new Date( ).getTime( )
console.log( 'Time in milliseconds: '+ ( end - start ) )

console.log( detections )

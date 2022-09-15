import cv2

video_capture = cv2.VideoCapture('C:/Users/Keegan Green/Downloads/confetti_1.mp4')

while(True):
    _, frame = video_capture.read()
    cv2.imshow('', frame)
    if cv2.waitKey(10) & 0xFF == ord('q'):
        break
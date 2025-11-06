# Simple Snake Game in Python 3 for Beginners
# By @TokyoEdTech
# modified by Yan Luo (with Arduino joystick + serial integration)

import turtle
import time
import random
import serial

# Serial port setup (update port as needed)
serialDevFile = '/dev/cu.usbmodem141101'  # Change this for your system
ser = serial.Serial(serialDevFile, 9600, timeout=0)

delay = 0.1
score = 0
high_score = 0
ppa = 10

# Set up screen
wn = turtle.Screen()
wn.title("Snake Game by @TokyoEdTech (mod by YL)")
wn.bgcolor("green")
wn.setup(width=600, height=600)
wn.tracer(0)

# Snake head
head = turtle.Turtle()
head.speed(0)
head.shape("square")
head.color("black")
head.penup()
head.goto(0, 0)
head.direction = "stop"

# Food
food = turtle.Turtle()
food.speed(0)
food.shape("circle")
food.color("red")
food.penup()
food.goto(0, 100)

segments = []

# Pen
pen = turtle.Turtle()
pen.speed(0)
pen.shape("square")
pen.color("white")
pen.penup()
pen.hideturtle()
pen.goto(0, 260)
pen.write("Score: 0  High Score: 0  P/A: 10", align="center", font=("Courier", 24, "normal"))

# Direction control
def go_up():    head.direction = "up"    if head.direction != "down" else None
def go_down():  head.direction = "down"  if head.direction != "up" else None
def go_left():  head.direction = "left"  if head.direction != "right" else None
def go_right(): head.direction = "right" if head.direction != "left" else None

def move():
    if head.direction == "up":    head.sety(head.ycor() + 20)
    if head.direction == "down":  head.sety(head.ycor() - 20)
    if head.direction == "left":  head.setx(head.xcor() - 20)
    if head.direction == "right": head.setx(head.xcor() + 20)

# Keyboard bindings (fallback)
wn.listen()
wn.onkey(go_up, "w")
wn.onkey(go_down, "s")
wn.onkey(go_left, "a")
wn.onkey(go_right, "d")

# Main loop
while True:
    wn.update()

    # --- READ JOYSTICK INPUT FROM ARDUINO ---
    if ser.in_waiting > 0:
        cmd = ser.read().decode('utf-8')
        if cmd == 'w' and head.direction != "down": head.direction = "up"
        elif cmd == 's' and head.direction != "up": head.direction = "down"
        elif cmd == 'a' and head.direction != "right": head.direction = "left"
        elif cmd == 'd' and head.direction != "left": head.direction = "right"

    # --- BORDER COLLISION ---
    if abs(head.xcor()) > 290 or abs(head.ycor()) > 290:
        time.sleep(1)
        head.goto(0, 0)
        head.direction = "stop"
        for segment in segments:
            segment.goto(1000, 1000)
        segments.clear()
        score = 0
        delay = 0.1
        pen.clear()
        pen.write("Score: {}  High Score: {}  P/A: {}".format(score, high_score, ppa),
                  align="center", font=("Courier", 24, "normal"))

    # --- APPLE COLLISION ---
    if head.distance(food) < 20:
        # Notify Arduino to beep
        ser.write(b'E')

        # Move apple
        x, y = random.randint(-290, 290), random.randint(-290, 290)
        food.goto(x, y)

        # Add segment
        new_segment = turtle.Turtle()
        new_segment.speed(0)
        new_segment.shape("square")
        new_segment.color("grey")
        new_segment.penup()
        segments.append(new_segment)

        # Update score
        score += 10
        if score > high_score:
            high_score = score

        pen.clear()
        pen.write("Score: {}  High Score: {}  P/A: {}".format(score, high_score, ppa),
                  align="center", font=("Courier", 24, "normal"))

    # Move body
    for i in range(len(segments) - 1, 0, -1):
        x = segments[i - 1].xcor()
        y = segments[i - 1].ycor()
        segments[i].goto(x, y)
    if len(segments) > 0:
        segments[0].goto(head.xcor(), head.ycor())

    move()

    # --- SELF COLLISION ---
    for segment in segments:
        if segment.distance(head) < 20:
            time.sleep(1)
            head.goto(0, 0)
            head.direction = "stop"
            for s in segments:
                s.goto(1000, 1000)
            segments.clear()
            score = 0
            delay = 0.1
            pen.clear()
            pen.write("Score: {}  High Score: {}  P/A: {}".format(score, high_score, ppa),
                      align="center", font=("Courier", 24, "normal"))

    time.sleep(delay)

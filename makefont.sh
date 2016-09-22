#!/bin/sh

printf '\xff\xff\xff\xff' > w
printf '\x00\x00\x00\xff' > b

cat b b b b b b b b b b b b b b b b b b b b > Space

cat b w w b w b b w w w w w w b b w w b b w > A
cat w w w b w b b w w w w b w b b w w w w b > B
cat b w w w w b b b w b b b w b b b b w w w > C
cat w w w b w b b w w b b w w b b w w w w b > D
cat w w w b w b b b w w w b w b b b w w w b > E
cat w w w b w b b b w w w b w b b b w b b b > F
cat b w w w w b b b w b w w w b b w b w w w > G
cat w b b w w b b w w w w w w b b w w b b w > H
cat b w b b b w b b b w b b b w b b b w b b > I
cat b b b w b b b w b b b w w b b w b w w b > J
cat w b b w w b w b w w b b w b w b w b b w > K
cat w b b b w b b b w b b b w b b b w w w b > L
cat w b b b w w w b w w w b w b w w b b b w > M
cat w b b w w w b w w b w w w b b w w b b w > N
cat b w w b w b b w w b b w w b b w b w w b > O
cat w w w b w b b w w w w b w b b b w b b b > P
cat b w w b w b b w w b b w w b w b b w b w > Q
cat w w w b w b b w w w w b w b w b w b b w > R
cat b w w w w b b b b w w b b b b w w w w b > S
cat w w w b b w b b b w b b b w b b b w b b > T
cat w b b w w b b w w b b w w b b w b w w b > U
cat w b b w w b b w w b b w b w w b b w w b > V
cat w w b w w w b w w w b w b w w b b w w b > W
cat w b b w w b b w b w w b w b b w w b b w > X
cat w b b w w b b w w w w w b b b w w w w b > Y
cat w w w w b b b w b w w b w b b b w w w w > Z

cat Space \
	Space Space Space Space Space Space Space Space Space Space Space Space Space Space Space \
	Space Space Space Space Space Space Space Space Space Space \
	Space Space Space Space Space Space Space \
	A B C D E F G H I J K L M N O P Q R S T U V W X Y Z > fontfile







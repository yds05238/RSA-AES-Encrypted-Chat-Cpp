#!/bin/bash

# check rsa_bits
if [ "$1" = 2048 ]; then 
    echo "Generating RSA 2048 keys"
elif [ "$1" = 4096 ]; then 
    echo "Generating RSA 4096 keys"
else
    echo "Invalid RSA bits value!"
    echo "Call generate_keys.sh rsa_bits with rsa_bits = 2048 or 4096."
    exit 
fi 

# key folder dir
keydir=$PWD
keyfile="$keydir/keys/rsa_key"

# clean up previous key files
rm -rf "$keydir/keys"
mkdir "$keydir/keys"

# Generate rsa private key file
ssh-keygen -P "" -t rsa -b $1 -m pem -f $keyfile
until [ -f "$keydir/keys/rsa_key" ]; do sleep 0.1; done 
cp -rf "$keydir/keys/rsa_key" "$keydir/keys/rsa_private.pem"

# Generate rsa public key file 
ssh-keygen -P "" -t rsa -b $1 -f $keyfile -e -m pem > "$keydir/keys/rsa_public.pem"
until [ -f "$keydir/keys/rsa_public.pem" ]; do sleep 0.1; done 

# clean up keys folder
rm -rf "$keydir/keys/rsa_key"
rm -rf "$keydir/keys/rsa_key.pub"

# ******************************
# Script per l'inizializzazione
# del cluster!
# ******************************

#!/bin/ssh


# ************** DEFINITION OF MASTER AND SLAVES **************

# MASTER
MASTER_IP=IP0
MASTER_USER_NAME="username0"
MASTER_HOST_NAME="hostname0" 

#HOST1
HOST1_IP=IP1
HOST1_USER_NAME="username1"
HOST1_HOST_NAME="hostname1"

#...

#HOSTN
# ...


# ************** ASSOCIATION BETWEEN OF MASTER AND SLAVES **************
# MASTER
ssh-keygen
cd ~
cd .ssh
cp id_rsa.pub $MASTER_HOST_NAME

# HOST1
ssh $HOST1_USER_NAME@$HOST1_IP
ssh-keygen
cd .ssh
cp id_rsa.pub $HOST1_HOST_NAME
scp $MASTER_IP:/home/$MASTER_USER_NAME/.ssh/$MASTER_HOST_NAME .
cat $MASTER_HOST_NAME >> authorized_keys
exit

# HOST2
# ssh $HOST2_USER_NAME@$HOST2_IP
# ssh-keygen
# cd .ssh
# cp id_rsa.pub $HOST2_HOST_NAME
# scp $MASTER_IP:/home/$MASTER_USER_NAME/.ssh/$MASTER_HOST_NAME .
# cat $MASTER_HOST_NAME >> authorized_keys
# exit

# ...

# HOSTN
# ...


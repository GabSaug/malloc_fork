#! /bin/sh

IFS=$'\n'
for cmd in $(cat test_cmds); do
  IFS=$' '
  ${cmd} > ref 2> /dev/null
  ret_ref=$?
  LD_PRELOAD='../libmalloc.so' ${cmd} > tes 2> /dev/null
  ret_tes=$?

  echo "-- test : ${cmd}"

  err=0
  if [ ! ${ret_ref} -eq ${ret_tes} ]; then
    echo "ERREUR : les valeurs de retour sont différentes"
    echo "${ret_tes} au lieu de ${ret_ref}"
    ((err++))
  fi
  diff -y --suppress-common-lines ref tes > result 
  if [ -s result ]; then
    echo "ERREUR : les sorties standards sont différentes"
  fi
  IFS=$'\n'
  for lines in $(cat result); do
    IFS=$' '
    echo "${lines}"
    ((err++))
  done

  echo ""
done

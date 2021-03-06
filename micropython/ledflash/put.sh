echo using environment defined port: [$PORT]
export baud=115200

export files=(
    "boot.py" 
    "main.py"
    )

for FILE in ${files[@]};
do
    ampy --port $PORT --baud $baud rm $FILE
done
for FILE in ${files[@]};
do
    ampy --port $PORT --baud $baud put $FILE
done

ampy --port $PORT reset
#pwd > $2
#echo "$1" >> $2
#echo "$2" >> $2

export PATH=~/Tools/:$PATH

#!/bin/bash
############ input parameters ################



FolderName="DAB1"
FolderName2="DAB1.1"
InputImage=$1
ImageName=$(basename $InputImage)
MacroName="DAB1.txt"
MacroName2="DAB1.1.txt"

echo $InputImage
echo $ImageName
echo $PWD

echo "---------------------------------------------------------------------------------------------------" >> log
echo "Commands that are going to be run: " > log
echo "mkdir $FolderName" >> log
echo "cp $InputImage $FolderName/" >> log
echo "ndpisplit \"$PWD/$FolderName/$ImageName\"" >> log
echo "mkdir macros" >> log
echo "split.out \"$PWD/$FolderName/\" \"$ImageName\" \"SplitRun.tmp\" \"RatioRun.txt\" \"$PWD/macros/\" \"$MacroName\" \"RatioResult.txt\" \"WidthHeight.txt\" \"2\" > $PWD/out.txt 2> $PWD/err.txt" >> log
echo "mv $PWD/$FolderName/$ImageName $PWD/$FolderName/$ImageName.ndpi" >> log
echo "cat $PWD/$FolderName/SplitRun.tmp | cut -d\"/\" -f2 >> $PWD/$FolderName/SplitRun.txt" >> log
echo "sed -i s%dataset_15.dat%$PWD/$FolderName/dataset_15.dat% $PWD/$FolderName/SplitRun.txt" >> log
echo "chmod 777 $PWD/$FolderName/SplitRun.txt" >> log
echo "$PWD/$FolderName/SplitRun.txt" >> log
echo "overlap.out \"$PWD/$FolderName/\" \"$ImageName\" \"2\" " >> log
echo "cp $PWD/macros/$MacroName ~/Tools/ImageJ/macros/" >> log
echo "WorkingDir=$PWD" >> log
echo "cd ~/Tools/ImageJ" >> log
echo "java -jar ij.jar -batch $MacroName" >> log
echo "cd $WorkingDir" >> log
echo "annotate.out \"$PWD/$FolderName/\" \"RatioResult.txt\" \"$ImageName\" \"2\" \"Top10.txt\" \"OnlyRatio.txt\"" >> log
echo "---------------------------------------------------------------------------------------------------" >> log


echo "Starting..." >> log

############ First part ################

echo "mkdir $FolderName" >> log
mkdir $FolderName

echo "cp $InputImage $FolderName/" >> log
cp $InputImage $FolderName/

echo "ndpisplit \"$PWD/$FolderName/$ImageName\"" >> log
ndpisplit "$PWD/$FolderName/$ImageName"			######  get *2.5 for dimension


############ Second part ################

echo "mkdir macros" >> log
mkdir macros

echo "split.out \"$PWD/$FolderName/\" \"$ImageName\" \"SplitRun.tmp\" \"RatioRun.txt\" \"$PWD/macros/\" \"$MacroName\" \"RatioResult.txt\" \"WidthHeight.txt\" \"2\" > $PWD/out.txt 2> $PWD/err.txt" >> log
split.out "$PWD/$FolderName/" "$ImageName" "SplitRun.tmp" "RatioRun.txt" "$PWD/macros/" "$MacroName" "RatioResult.txt" "WidthHeight.txt" "2" > $PWD/out.txt 2> $PWD/err.txt

echo "mv $PWD/$FolderName/$ImageName $PWD/$FolderName/$ImageName.ndpi" >> log
mv $PWD/$FolderName/$ImageName $PWD/$FolderName/$ImageName.ndpi

echo "cat $PWD/$FolderName/SplitRun.tmp | cut -d\"/\" -f2 >> $PWD/$FolderName/SplitRun.txt" >> log
cat $PWD/$FolderName/SplitRun.tmp | cut -d"/" -f2 >> $PWD/$FolderName/SplitRun.txt

echo "sed -i s%$ImageName%$PWD/$FolderName/$ImageName% $PWD/$FolderName/SplitRun.txt" >> log
sed -i s%$ImageName%$PWD/$FolderName/$ImageName% $PWD/$FolderName/SplitRun.txt
############ Third part ################

echo "chmod 777 $PWD/$FolderName/SplitRun.txt" >> log
chmod 777 $PWD/$FolderName/SplitRun.txt

echo "$PWD/$FolderName/SplitRun.txt" >> log
$PWD/$FolderName/SplitRun.txt

echo "overlap.out \"$PWD/$FolderName/\" \"$ImageName\" \"2\" " >> log
overlap.out "$PWD/$FolderName/" "$ImageName" "2" 

############ Forth part ################


echo "cp $PWD/macros/$MacroName ~/Tools/ImageJ/macros/" >> log
cp $PWD/macros/$MacroName ~/Tools/ImageJ/macros/

echo "WorkingDir=$PWD" >> log
WorkingDir=$PWD

echo "cd ~/Tools/ImageJ" >> log
cd ~/Tools/ImageJ

echo "java -jar ij.jar -batch $MacroName" >> log
java -jar ij.jar -batch $MacroName

echo "cd $WorkingDir" >> log
cd $WorkingDir

############ Fifth part ################

echo "annotate.out \"$PWD/$FolderName/\" \"RatioResult.txt\" \"$ImageName\" \"2\" \"Top10.txt\" \"OnlyRatio.txt\"" >> log
annotate.out "$PWD/$FolderName/" "RatioResult.txt" "$ImageName" "2" "Top10.txt" "OnlyRatio.txt"



############ Html output ################

outputHtml=$2
outputDir=$3
mkdir $outputDir

top10=`sed -n '11,20p' $PWD/DAB1/Top10.txt | grep -o "^[0-9]*"`
cp $PWD/DAB1/Top10.txt $outputDir
count=1
cp $PWD/DAB1/AnnotatedDAB.jpg $outputDir
echo "<html><h1>Overview:</h1><img src='AnnotatedDAB.jpg' width='1280' height='720'/><br/><a href='Top10.txt'><h1>Top 10 images: </h1></a>" > $outputHtml
for value in $top10; do
	cp $PWD/DAB1/$value.jpg $outputDir
	echo "<h1>$count:</h1>" >> $outputHtml
	echo "<img src='$value.jpg' width='1280' height='720'/>" >> $outputHtml
	count=$((count+1))
done


echo "<a href='Top10CellCount.txt'><h1>Top 10 Cell Count:</h1></a>" >> $outputHtml
~/ExportImages/cellcount1 $PWD/DAB1/ Top10.txt Top10CellCount.txt exportimages
cp $PWD/DAB1/Top10CellCount.txt  $outputDir
count=1
while read img score
do
	cp $PWD/DAB1/${img}.jpg $outputDir
	echo "<h1>$count: $score</h1>" >> $outputHtml
	echo "<img src='$img.jpg' width='1280' height='720'/>" >> $outputHtml
	count=$((count+1))
done < $outputDir/Top10CellCount.txt




echo "</html>" >> $outputHtml


############ Sixth part ################   Adaptive step finding
#mkdir ~/Ki67Analysis/$FolderName2
		
#./adapt.out "/home/testki67/Ki67Analysis/$FolderName/" "$ImageName" "2" "Top10.txt" "8" "/home/testki67/Ki67Analysis/$FolderName2/" "RatioRun2.txt" "/home/testki67/#Tools/ImageJ/macros/" "$MacroName2" "RatioResult2.txt"

#cd ~/Tools/ImageJ/macros
#chmod 777 $MacroName2
#cd ..
#java -jar ij.jar -batch $MacroName2

#cd ~/Tools  		
#./show.out "/home/testki67/Ki67Analysis/$FolderName/" "$ImageName" "2" "8" "/home/testki67/Ki67Analysis/$FolderName2/" "RatioResult2.txt" "Top10.txt" 






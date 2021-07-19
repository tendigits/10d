#!/bin/bash
# Ten Digits image processing script, based on: https://git.sr.ht/~rostiger/nchrs/blob/main/src/batchVariants.sh
SRC="img"
DST="../assets/img"
SIZES=(400)
MAXWIDTH=1600
COLORS=4
TINTCOLOR='wheat'

function resize () {
	
  # prevent an endless loop 
  [[ $file == *"${DST}/*"* ]] && continue 

  for file in $1
  do
	  [ -f "$file" ] || continue

	# create output path
	path=$(dirname $file)   # just/the/path    
	name=$(basename $file)  # filename.ext
	fileBase="${name%%.*}"  # filename
	fileExt="${name#*.}"    # ext

	dst="${path/$SRC/$DST}"    

	# existing images are skipped (delete images if they were updated)    
	# create the output path (and parents) if it doesn't exist
	if [[ ! -d "$dst" ]]; then 
	  mkdir -p $dst
	fi
		
	# copy the file as is if it doesn't have the right extension
	if [[ "$fileExt" != "jpg" && "$fileExt" != "jpeg" && "$fileExt" != "png" ]]; then
			cp -r $file $dst
		   echo "Copied ${file}"
			continue
	fi

	# create smaller size
	echo $file
	width=$(identify -format "%w" "$file")> /dev/null
	  for size in "${SIZES[@]}"
	  do
		output="$dst/$fileBase-low.png"
	  if [[ ! -f $output ]]; then
		# resize only if bigger
		if [[ $width -ge $size ]]; then
		  echo -n "| ${size} "
				convert $file -strip -auto-orient -resize $size -dither FloydSteinberg -colorspace Gray -fill $TINTCOLOR -tint 90 -colors $COLORS $output
		else
			#dither only
		  echo -n "| ${width} "
				convert $file -strip -auto-orient -dither FloydSteinberg -colorspace Gray -fill $TINTCOLOR -tint 90 -colors $COLORS $output
			fi
	  else echo -n "| ----- "
	  fi
	done
	# Strip EXIF data and resize high res image
	output="$dst/$name"
	if [[ ! -f $output ]]; then
		if [[ $width -gt $MAXWIDTH ]]; then
		  convert $file -strip -auto-orient -resize $MAXWIDTH $output
		  echo -n "| ${MAXWIDTH} "
	  else
		  convert $file -strip -auto-orient $output
		  echo -n "| ${width} "
	  fi
	else echo -n "| ----- "
	fi
	echo -en "|\n"
  done
}

# find all file in the source folder and run resize() on each
find $SRC | while read file; do resize "${file}"; done

#!/usr/bin/env python3
"""
Create optimized sprite sheet from individual frames for TB-303 Distortion plugin
"""

from PIL import Image
import os
import glob

def create_sprite_sheet():
    # Source directory with individual frames
    source_dir = "/Users/muster/Desktop/Smiley Kopie 2"
    
    # Get all JPG files and sort them by name
    frame_files = sorted(glob.glob(os.path.join(source_dir, "*.jpg")))
    
    if not frame_files:
        print("No JPG files found!")
        return
    
    print(f"Found {len(frame_files)} frames")
    
    # High resolution frame size for crisp quality
    frame_size = (300, 300)
    
    # Create horizontal sprite sheet
    total_width = frame_size[0] * len(frame_files)
    total_height = frame_size[1]
    
    print(f"Creating sprite sheet: {total_width}x{total_height}")
    
    # Create new image with transparent background
    sprite_sheet = Image.new('RGBA', (total_width, total_height), (0, 0, 0, 0))
    
    # Process each frame
    for i, frame_file in enumerate(frame_files):
        try:
            # Load and process frame
            frame = Image.open(frame_file)
            
            # Convert to RGBA if needed
            if frame.mode != 'RGBA':
                frame = frame.convert('RGBA')
            
            # Resize to plugin size with high quality
            frame_resized = frame.resize(frame_size, Image.Resampling.LANCZOS)
            
            # Paste into sprite sheet
            x_position = i * frame_size[0]
            sprite_sheet.paste(frame_resized, (x_position, 0))
            
            print(f"  Frame {i+1}: {os.path.basename(frame_file)} -> position {x_position}")
            
        except Exception as e:
            print(f"Error processing {frame_file}: {e}")
            continue
    
    # Save the sprite sheet
    output_file = "/Users/muster/Documents/GitHub/Juice/Juice/plugins/TB303distortion/NewProject/Resources/smiley_mega_sprite_sheet.png"
    sprite_sheet.save(output_file, 'PNG', optimize=True)
    
    print(f"\nSprite sheet created: {output_file}")
    print(f"Dimensions: {sprite_sheet.size}")
    print(f"Frames: {len(frame_files)}")
    print(f"Frame size: {frame_size}")
    
    return len(frame_files)

if __name__ == "__main__":
    num_frames = create_sprite_sheet()
    print(f"\nReady for {num_frames}-frame animation!")
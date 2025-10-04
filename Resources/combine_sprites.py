#!/usr/bin/env python3
"""
Combine 7 sprite sheets into one mega 42-frame sprite sheet for TB-303 Distortion plugin
"""

from PIL import Image
import sys

def combine_sprite_sheets():
    # Load all 7 sprite sheet images from your screenshots
    sprite_files = [
        "/Users/muster/Downloads/CleanShot 2025-10-02 at 23.48.01@2x.png",  # Image 1
        "/Users/muster/Downloads/CleanShot 2025-10-02 at 23.48.03@2x.png",  # Image 2  
        "/Users/muster/Downloads/CleanShot 2025-10-02 at 23.48.05@2x.png",  # Image 3
        "/Users/muster/Downloads/CleanShot 2025-10-02 at 23.48.07@2x.png",  # Image 4
        "/Users/muster/Downloads/CleanShot 2025-10-02 at 23.48.08@2x.png",  # Image 5
        "/Users/muster/Downloads/CleanShot 2025-10-02 at 23.48.10@2x.png",  # Image 6
        "/Users/muster/Downloads/CleanShot 2025-10-02 at 23.48.12@2x.png",  # Image 7
    ]
    
    # Each sprite sheet has 6 frames in 3x2 layout
    frames_per_sheet = 6
    total_frames = len(sprite_files) * frames_per_sheet  # 42 frames
    
    # Load first image to get dimensions
    first_img = Image.open(sprite_files[0])
    sheet_width, sheet_height = first_img.size
    
    # Calculate individual frame size (assuming 3x2 layout)
    frame_width = sheet_width // 3
    frame_height = sheet_height // 2
    
    print(f"Sheet dimensions: {sheet_width}x{sheet_height}")
    print(f"Frame dimensions: {frame_width}x{frame_height}")
    print(f"Total frames: {total_frames}")
    
    # Create mega sprite sheet layout: 6 columns x 7 rows = 42 frames
    mega_width = frame_width * 6
    mega_height = frame_height * 7
    mega_sheet = Image.new('RGBA', (mega_width, mega_height), (0, 0, 0, 0))
    
    print(f"Mega sheet dimensions: {mega_width}x{mega_height}")
    
    frame_index = 0
    
    # Process each sprite sheet
    for sheet_idx, sprite_file in enumerate(sprite_files):
        try:
            img = Image.open(sprite_file)
            print(f"Processing sheet {sheet_idx + 1}: {sprite_file}")
            
            # Extract 6 frames from current sheet (3x2 layout)
            for row in range(2):
                for col in range(3):
                    # Extract frame from source sheet
                    left = col * frame_width
                    top = row * frame_height
                    right = left + frame_width
                    bottom = top + frame_height
                    
                    frame = img.crop((left, top, right, bottom))
                    
                    # Place frame in mega sheet (6 columns layout)
                    mega_col = frame_index % 6
                    mega_row = frame_index // 6
                    
                    mega_x = mega_col * frame_width
                    mega_y = mega_row * frame_height
                    
                    mega_sheet.paste(frame, (mega_x, mega_y))
                    
                    print(f"  Frame {frame_index + 1}: ({col},{row}) -> ({mega_col},{mega_row})")
                    frame_index += 1
                    
        except Exception as e:
            print(f"Error processing {sprite_file}: {e}")
            continue
    
    # Save the mega sprite sheet
    output_file = "/Users/muster/Documents/GitHub/Juice/Juice/plugins/TB303distortion/NewProject/Resources/smiley_mega_sprite_sheet.png"
    mega_sheet.save(output_file, 'PNG')
    print(f"\nMega sprite sheet saved: {output_file}")
    print(f"Final dimensions: {mega_sheet.size}")
    
    return output_file

if __name__ == "__main__":
    combine_sprite_sheets()
import code
import math
from itertools import batched
from pathlib import Path
from PIL import Image, ImageDraw, ImageFont
from PIL.ImageFile import ImageFile

def reload():
    exec(open(__file__).read(), globals())

#Helpers
def expand_transp(image: ImageFile, pix_thick: int):

    bg = Image.new("RGBA", (image.width + pix_thick * 2, image.height + pix_thick * 2), (0,0,0,0))
    bg.paste(image, (pix_thick,) * 2, image)

    return bg
def set_scale(image: ImageFile, factor: float):
    scaled = image.resize(
        (
            round(image.width * factor),
            round(image.height * factor)
        )
    )
    return scaled
def centered_coords(img1: ImageFile, img2: ImageFile):
    return round((img2.width - img1.width) * 0.5), round((img2.height - img1.height) * 0.5)
def load(path: str):
    with Image.open(path) as img:
        return img.copy()

#Path widgets
widgets_dir = Path(r"C:\Users\User\Pictures\DST Widgets")

#Make fonts
font_dir = r"C:\Users\User\AppData\Local\Microsoft\Windows\Fonts\belisa_plumilla.ttf"
font_small = ImageFont.truetype(font_dir, 44)
font_big = ImageFont.truetype(font_dir, 80)

#Path items
item_paths = {path.stem: path
              for path in (widgets_dir / "[Materials]" / "Items").rglob("*.png")
              }
#Create item cache
items = {}

#Load canvases
canvases = { name: load(widgets_dir / f"{name}.png") for name in ("blank_table",)}

#Load item box
box: ImageFile = expand_transp(load(widgets_dir / "[Materials]" / "Raw Icon" / "item_square.png"), 20)

#Define item factory
def item(name: str, quantity = 1):
    #Cache framed item
    if name in items:
        item = items[name].copy()
    else:
        item = box.copy()

        with Image.open(item_paths[name]) as bare:
            #Place item on grid
            bare = set_scale(bare, 1.2)
            item.paste(bare, centered_coords(bare, item), bare)

            items[name] = item

        item = item.copy()

    #Text quantity
    ImageDraw.Draw(item).text((95, 76), f"x{quantity}", font=font_small, stroke_fill="black", stroke_width=1.9)

    return item

#Define table factory
def table(title: str, *items: ImageFile):
    #Error check
    if len(items) > 12:
        raise ValueError("Too Many Items.")

    #Alias canvas & draw
    canvas = canvases["blank_table"].copy()
    draw = ImageDraw.Draw(canvas)

    #Def canvas center
    canv_center = (canvas.width // 2, canvas.height // 1.7666)

    #Draw title
    title_center = ((canvas.width - draw.textlength(title, font=font_big)) * 0.5, 45)
    draw.text(title_center, title, font=font_big, stroke_fill="black", stroke_width=2.36)

    #Find ceils
    row_ceil = math.ceil(math.sqrt(len(items)))
    square_ceil = max(1, row_ceil - 1)
    f = square_ceil**0.6
    #Scale by square ceil
    scale = 1.1666 / f

    #Def spacing factor
    spacing_fac = 1 / f

    #Find rows
    rows = math.ceil(len(items) / row_ceil)

    #Paste Items
    for row_i, row in enumerate(batched(items, row_ceil)):
        y = canv_center[1] + (row_i - (rows - 1) / 2) * 120 * spacing_fac
        k = len(row);

        for item_i, item in enumerate(row):
            x = canv_center[0] + (item_i - (k - 1) / 2) * 120 * spacing_fac

            item = set_scale(item, scale)
            canvas.paste(item,
                         (
                             round(x - item.width / 2),
                             round(y - item.height / 2)
                         ),
                         item
            )

    canvas.save("test.png", "png")

#Start REPL
# code.interact(local=globals(),
#               banner="WidGen for DST: Table\t (Ctrl+D to Exit)\n",
#               exitmsg="")

table("test", item("twigs", 4), item("tusk", 3), item("gears", 2), item("fossils"), item("fossils"), item("fossils"), item("fossils"),
      item("fossils"), item("fossils"), item("fossils"), item("fossils"))

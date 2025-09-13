import pygame
import sys

PALETTE = [
    (0, 0, 0), # Black
    (128, 0, 0), # Red
    (0, 128, 0), # Green
    (128, 128, 0), # Yellow
    (0, 0, 128), # Blue
    (128, 0, 128), # Magenta
    (0, 128, 128), # Cyan
    (192, 192, 192) # White/Gray
]

PALETTE_SIZE = 40
PALETTE_PADDING = 5
BUTTON_HEIGHT = 30
BUTTON_WIDTH = 100

class PixelEditor:
    def __init__(self):
        self.width = int(input("Width in pixels: "))
        self.height = int(input("Height in pixels: "))
        
        self.cell_size = 20
        self.h_squash = 0.5
        self.cell_width = int(self.cell_size * self.h_squash)
        self.cell_height = self.cell_size
        
        self.drawing_width = self.width * self.cell_width
        self.drawing_height = self.height * self.cell_height
        
        self.palette_cols = len(PALETTE)
        self.palette_width = self.palette_cols * (PALETTE_SIZE + PALETTE_PADDING)
        
        self.screen_width = self.drawing_width + self.palette_width + 20
        self.screen_height = max(self.drawing_height, PALETTE_SIZE*2 + BUTTON_HEIGHT + 10)
        
        pygame.init()
        self.screen = pygame.display.set_mode((self.screen_width, self.screen_height))
        pygame.display.set_caption("Pixel Editor")
        self.clock = pygame.time.Clock()
        
        self.grid = [[0 for _ in range(self.width)] for _ in range(self.height)]
        self.current_color = 7
        
        self.button_rect = pygame.Rect(self.drawing_width + 10,
                                       PALETTE_SIZE + 20,
                                       BUTTON_WIDTH,
                                       BUTTON_HEIGHT)

        self.run()

    def run(self):
        running = True
        while running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False
                elif event.type == pygame.MOUSEBUTTONDOWN or pygame.mouse.get_pressed()[0]:
                    self.handle_mouse(pygame.mouse.get_pos())
                elif event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_s:
                        self.export_png()
            self.render()
            pygame.display.flip()
            self.clock.tick(60)
        pygame.quit()
        sys.exit()

    def handle_mouse(self, pos):
        x, y = pos
        
        if self.button_rect.collidepoint(x, y):
            self.export_png()
            return
        
        palette_x_start = self.drawing_width + 10
        palette_y_start = 10
        for idx, color in enumerate(PALETTE):
            px = palette_x_start + idx * (PALETTE_SIZE + PALETTE_PADDING)
            py = palette_y_start
            rect = pygame.Rect(px, py, PALETTE_SIZE, PALETTE_SIZE)
            if rect.collidepoint(x, y):
                self.current_color = idx
                return
        
        if 0 <= x < self.drawing_width and 0 <= y < self.drawing_height:
            px = x // self.cell_width
            py = y // self.cell_height
            if 0 <= px < self.width and 0 <= py < self.height:
                self.grid[py][px] = self.current_color

    def render(self):
        self.screen.fill((30,30,30))
        
        pygame.draw.rect(self.screen, (50,50,50), (0,0,self.drawing_width,self.drawing_height))
        
        for y in range(self.height):
            for x in range(self.width):
                color = PALETTE[self.grid[y][x]]
                rect = pygame.Rect(x*self.cell_width, y*self.cell_height, self.cell_width, self.cell_height)
                pygame.draw.rect(self.screen, color, rect)
        
        pygame.draw.rect(self.screen, (255,255,255), (0,0,self.drawing_width,self.drawing_height), 2)
        
        palette_x_start = self.drawing_width + 10
        palette_y_start = 10
        for idx, color in enumerate(PALETTE):
            px = palette_x_start + idx * (PALETTE_SIZE + PALETTE_PADDING)
            py = palette_y_start
            rect = pygame.Rect(px, py, PALETTE_SIZE, PALETTE_SIZE)
            pygame.draw.rect(self.screen, color, rect)
            if idx == self.current_color:
                pygame.draw.rect(self.screen, (255,255,255), rect, 3)
                
        pygame.draw.rect(self.screen, (70,70,70), self.button_rect)
        font = pygame.font.SysFont(None, 24)
        text = font.render("Export PNG", True, (255,255,255))
        text_rect = text.get_rect(center=self.button_rect.center)
        self.screen.blit(text, text_rect)

    def export_png(self):
        img = pygame.Surface((self.width, self.height))
        for y in range(self.height):
            for x in range(self.width):
                img.set_at((x, y), PALETTE[self.grid[y][x]])
        fname = input("Enter PNG filename to export: ").strip()
        pygame.image.save(img, fname)
        print(f"Saved PNG as {fname}")

if __name__ == "__main__":
    PixelEditor()
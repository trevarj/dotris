# Arch User Repository PKGBUILD
# Maintainer: Your Name <tmarjeski at gmail dot com>
pkgname=dotris
pkgver=0.1.0
pkgrel=1
pkgdesc="A mini Tetris game made with dots"
arch=('any')
url="https://github.com/trevarj/dotris"
license=('GPL')
depends=('ncurses')
makedepends=('git')
source=(git+$url)
sha256sums=('SKIP')

build() {
	cd "$pkgname"
	make
}

package() {
	cd "$pkgname"
	make DESTDIR="$pkgdir/" PREFIX="/usr" install
	install -Dm644 LICENSE "$pkgdir/usr/share/licenses/$pkgname/LICENSE"
}
